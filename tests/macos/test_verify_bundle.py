import sys
import unittest
from pathlib import Path
from verify_bundle import smoke_test, validate_dependencies


class DependenciesTest(unittest.TestCase):
    def test_rejects_agl_in_platform_plugin(self):
        with self.assertRaisesRegex(ValueError, "Obsolete macOS 26"):
            validate_dependencies("libqcocoa.dylib:\n\t/System/Library/Frameworks/AGL.framework/Versions/A/AGL (compatibility version 1.0.0)")

    def test_rejects_runner_library(self):
        with self.assertRaisesRegex(ValueError, "Non-portable"):
            validate_dependencies("jumble:\n\t/Users/runner/Qt/lib/QtCore (compatibility version 1.0.0)")

    def test_accepts_bundled_and_system_libraries(self):
        validate_dependencies("jumble:\n\t@rpath/QtCore.framework/Versions/A/QtCore (compatibility version 6.0.0)\n\t/usr/lib/libSystem.B.dylib (compatibility version 1.0.0)")

    def test_rejects_app_that_exits_early(self):
        # Starting Python without input exits immediately, like a broken bundle.
        from unittest.mock import patch
        import subprocess
        original = subprocess.Popen
        with patch("verify_bundle.subprocess.Popen", side_effect=lambda args, **kw: original([sys.executable, "-c", "raise SystemExit(6)"], **kw)):
            with self.assertRaisesRegex(RuntimeError, "exited early: 6"):
                smoke_test(Path(sys.executable), seconds=2)


if __name__ == "__main__":
    unittest.main()