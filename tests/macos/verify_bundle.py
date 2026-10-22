"""Check the extracted Mac download, including native Cocoa startup."""
import os
from pathlib import Path
import subprocess
import sys
import tempfile


def validate_dependencies(output):
    for line in output.splitlines()[1:]:
        dependency = line.strip().split(" (", 1)[0]
        if not dependency:
            continue
        if "/AGL.framework/" in dependency:
            raise ValueError(f"Obsolete macOS 26 dependency: {dependency}")
        if not dependency.startswith(("@rpath/", "@loader_path/", "@executable_path/",
                                      "/System/Library/", "/usr/lib/")):
            raise ValueError(f"Non-portable dependency: {dependency}")


def smoke_test(executable, seconds=10):
    # An installed Qt SDK must not mask missing plugins in the download.
    env = {k: v for k, v in os.environ.items()
           if not k.startswith(("QT_", "QML", "DYLD_"))}
    env["QT_QPA_PLATFORM"] = "cocoa"
    env["QT_DEBUG_PLUGINS"] = "1"
    with tempfile.TemporaryDirectory() as cwd, tempfile.TemporaryFile() as log:
        process = subprocess.Popen([str(executable)], cwd=cwd, env=env,
                                   stdout=log, stderr=subprocess.STDOUT)
        try:
            try:
                code = process.wait(timeout=seconds)
            except subprocess.TimeoutExpired:
                print(f"Packaged app launched with Cocoa and stayed alive for {seconds}s")
            else:
                raise RuntimeError(f"Packaged app exited early: {code}")
        finally:
            if process.poll() is None:
                process.terminate()
                try:
                    process.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    process.kill()
                    process.wait()
            log.seek(0)
            print(log.read().decode(errors="replace"))


def verify_bundle(app):
    executable = app / "Contents/MacOS/jumble"
    if not executable.is_file() or not os.access(executable, os.X_OK):
        raise RuntimeError("Missing executable in extracted app")
    if not (app / "Contents/PlugIns/platforms/libqcocoa.dylib").is_file():
        raise RuntimeError("Missing bundled Cocoa platform plugin")
    architectures = subprocess.check_output(["lipo", "-archs", str(executable)], text=True).split()
    if "x86_64" not in architectures:
        raise RuntimeError(f"Intel slice missing: {architectures}")
    subprocess.run(["codesign", "--verify", "--deep", "--strict", str(app)], check=True)
    for path in app.rglob("*"):
        if path.is_file() and not path.is_symlink():
            kind = subprocess.check_output(["file", "-b", str(path)], text=True)
            if "Mach-O" in kind:
                validate_dependencies(subprocess.check_output(["otool", "-L", str(path)], text=True))
    smoke_test(executable)


if __name__ == "__main__":
    verify_bundle(Path(sys.argv[1]).resolve())