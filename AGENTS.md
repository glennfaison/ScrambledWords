# Build and deployment rules

- Keep Qt 5 compatibility and Linux screenshot baselines; deploy macOS with a Qt release whose Cocoa plugin does not link AGL, because macOS 26 removed that framework.
- Validate dependencies, signatures, Intel architecture, and native Cocoa startup on the extracted macOS archive before uploading it; a successful compile does not establish that the delivered app can launch.