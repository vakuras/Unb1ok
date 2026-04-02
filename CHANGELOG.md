# Changelog

## 2.0.0 — 2026-04-02

Complete rewrite from C++/Win32/OpenGL to TypeScript/Three.js for the web.

### Added
- Browser-based gameplay — zero installation, works on any device
- Three.js renderer with PBR materials, soft shadows, and environment lighting
- Original skybox from the 2010 version
- Original sound effects (OGG) played via Web Audio API
- OrbitControls camera (drag to orbit, scroll to zoom)
- Raycaster-based block picking and constrained-axis dragging
- AABB collision detection (block vs block, block vs board boundary)
- HTML/CSS overlay UI: menu screen, level HUD, win celebration
- Smooth spring animations for block snapping
- localStorage progress saving (last level played)
- Level skip with +/- keys
- 1200 levels extracted and preserved from original game

### Removed
- C++ source code and Win32/OpenGL engine (`vEngine`, `vutil`)
- Dependency on OpenAL 1.1 SDK, libogg, libvorbis
- Visual Studio project files (`.vcxproj`, `.sln`)
- Windows-only executable

### Changed
- Build system: Visual Studio → Vite + TypeScript
- Rendering: Legacy OpenGL fixed-function pipeline → Three.js WebGL2
- Audio: OpenAL → Web Audio API (graceful fallback if unavailable)
- Block picking: `glRenderMode(GL_SELECT)` → Three.js Raycaster
- Settings storage: Binary `.cfg` file → localStorage
- Asset loading: ZIP-embedded Win32 resources → Vite-bundled static assets

---

## 1.0.0 — 2010

Original release. Windows-only C++ application.

- Custom 3D engine (`vEngine`) with OpenGL fixed-function pipeline
- OpenAL 1.1 audio with OGG Vorbis decoding
- 1200 puzzle levels
- Skybox, textured blocks, 3D orbiting camera
- Mouse-based block dragging with OpenGL selection mode
- Fullscreen/windowed mode toggle
- Settings saved to `unb1ok.cfg`
