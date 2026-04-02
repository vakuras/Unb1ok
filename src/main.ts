import { Scene as GameScene } from './Scene';
import { Board } from './Board';
import { Level } from './Level';
import { Input } from './Input';
import { UI } from './UI';
import { Audio } from './Audio';

type GameState = 'menu' | 'playing' | 'win';

class Game {
  private scene: GameScene;
  private board: Board;
  private level: Level;
  private input: Input;
  private ui: UI;
  private audio: Audio;
  private state: GameState = 'menu';
  private clock = { last: 0 };

  constructor() {
    const canvas = document.getElementById('game-canvas') as HTMLCanvasElement;
    this.scene = new GameScene(canvas);
    this.board = new Board();
    this.level = new Level();
    this.ui = new UI();
    this.audio = new Audio();
    this.input = new Input(this.scene, () => this.level.blocks);

    // Load saved progress
    const saved = localStorage.getItem('unb1ok-level');
    if (saved) {
      this.level.currentIndex = Math.max(0, parseInt(saved, 10) || 0);
    }

    this.setupCallbacks();
    this.setupKeyboard();

    // Show board in menu background
    this.board.addToScene(this.scene.scene);

    // Start render loop
    this.clock.last = performance.now();
    this.animate();
  }

  private setupCallbacks() {
    this.ui.onPlay = () => this.startGame();
    this.ui.onNext = () => this.nextLevel();
    this.input.onWin = () => this.onWin();
    this.input.onSlide = () => this.audio.slide();
  }

  private setupKeyboard() {
    window.addEventListener('keydown', (e) => {
      if (e.key === 'Escape' && this.state === 'playing') {
        this.goToMenu();
      }
      if (e.key === '+' || e.key === '=') {
        if (this.state === 'playing') {
          this.level.nextLevel(this.scene.scene);
          this.ui.updateLevel(this.level.currentIndex + 1);
        }
      }
      if (e.key === '-') {
        if (this.state === 'playing') {
          this.level.prevLevel(this.scene.scene);
          this.ui.updateLevel(this.level.currentIndex + 1);
        }
      }
    });
  }

  private startGame() {
    this.state = 'playing';
    this.level.load(this.level.currentIndex, this.scene.scene);
    this.ui.showGame(this.level.currentIndex + 1);
    this.audio.select();
  }

  private onWin() {
    if (this.state !== 'playing') return;
    this.state = 'win';
    this.audio.win();

    // Save progress
    const nextIndex = this.level.currentIndex + 1;
    localStorage.setItem('unb1ok-level', String(nextIndex));

    // Delay showing win screen for dramatic effect
    setTimeout(() => {
      this.ui.showWin();
    }, 600);
  }

  private nextLevel() {
    this.state = 'playing';
    this.level.nextLevel(this.scene.scene);
    this.ui.showGame(this.level.currentIndex + 1);
    this.audio.select();
  }

  private goToMenu() {
    this.state = 'menu';
    this.level.unload(this.scene.scene);
    this.ui.showMenu();
  }

  private animate() {
    requestAnimationFrame(() => this.animate());

    const now = performance.now();
    const dt = Math.min((now - this.clock.last) / 1000, 0.05);
    this.clock.last = now;

    if (this.state === 'playing' || this.state === 'win') {
      this.level.update(dt);
    }

    this.scene.render();
  }
}

// Boot
new Game();
