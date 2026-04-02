# Unb1ok

A 3D sliding-block puzzle game — slide the red block out through the exit gap.

**[▶ Play in your browser](https://vakuras.github.io/Unb1ok/)**

![Unb1ok Screenshot](https://img.shields.io/badge/levels-1200-blue) ![TypeScript](https://img.shields.io/badge/TypeScript-Three.js-orange) ![License](https://img.shields.io/badge/license-MIT-green)

## About

Unb1ok is a "sliding block" puzzle game where you drag blocks on a 6×6 board to clear a path for the red hero block to escape through the exit on the right side. It features 1200 levels of increasing difficulty.

## History

**2010** — Originally written in C++ for Windows by Vadim Kuras. Built on a custom mini 3D engine (`vEngine`) using the Win32 API, legacy OpenGL fixed-function pipeline, GLU, and OpenAL 1.1 for audio. Textures and levels were packed into ZIP files embedded as Win32 resources. The game featured a skybox, textured crate blocks, an orbiting 3D camera, and mouse-based dragging with OpenGL selection-mode picking (`glRenderMode(GL_SELECT)`).

**2017** — Published on GitHub under the MIT license.

**2026** — Completely rewritten as a modern web application using TypeScript and Three.js. The original 1200 levels were extracted and preserved. The new version runs in any browser with zero installation — just open a URL. The 3D camera and drag-to-slide interaction model were kept faithful to the original, while the visuals were modernized with PBR materials, soft shadows, and the original skybox.

## Tech Stack

| Then (2010) | Now (2026) |
|---|---|
| C++ / Win32 API | TypeScript |
| Legacy OpenGL + GLU | Three.js (WebGL2) |
| Custom `vEngine` renderer | Three.js Scene + PBR materials |
| `glRenderMode(GL_SELECT)` | Three.js Raycaster |
| OpenAL 1.1 + OGG Vorbis | Web Audio API + original OGG files |
| ZIP-embedded Win32 resources | Vite bundled assets |
| `.cfg` binary file | localStorage |
| Windows-only `.exe` | Any browser, any device |

## Getting Started

### Play Online

Just visit the GitHub Pages link above — no installation needed.

### Run Locally

```bash
git clone https://github.com/vakuras/Unb1ok.git
cd Unb1ok
npm install
npm run dev
```

Open `http://localhost:3000` in your browser.

### Build for Production

```bash
npm run build
```

Output goes to `dist/` — deploy to any static host.

## Controls

| Input | Action |
|---|---|
| **Left-click + drag** | Slide a block along its axis |
| **Right-click + drag** | Orbit camera |
| **Scroll wheel** | Zoom in/out |
| **ESC** | Return to menu |
| **+** / **-** | Skip to next / previous level |

## Project Structure

```
src/
├── main.ts        Entry point, game loop
├── Scene.ts       Three.js renderer, camera, skybox, lights
├── Board.ts       6×6 game board with walls and exit gap
├── Block.ts       Block mesh, PBR materials, movement, collision
├── Level.ts       Level loading/unloading
├── levels.ts      All 1200 levels (extracted from original)
├── Input.ts       Mouse/touch raycasting and drag handling
├── UI.ts          HTML overlay (menu, HUD, win screen)
├── Audio.ts       Web Audio with original OGG sound effects
└── styles.css     UI styling
```

## License

MIT — see [LICENSE](LICENSE)

Originally written by Vadim Kuras, 2010.
