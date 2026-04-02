export class UI {
  private menuScreen: HTMLElement;
  private hudScreen: HTMLElement;
  private winScreen: HTMLElement;
  private levelDisplay: HTMLElement;
  private btnPlay: HTMLElement;
  private btnNext: HTMLElement;

  onPlay: (() => void) | null = null;
  onNext: (() => void) | null = null;

  constructor() {
    this.menuScreen = document.getElementById('menu-screen')!;
    this.hudScreen = document.getElementById('hud')!;
    this.winScreen = document.getElementById('win-screen')!;
    this.levelDisplay = document.getElementById('level-display')!;
    this.btnPlay = document.getElementById('btn-play')!;
    this.btnNext = document.getElementById('btn-next')!;

    this.btnPlay.addEventListener('click', () => this.onPlay?.());
    this.btnNext.addEventListener('click', () => this.onNext?.());
  }

  showMenu() {
    this.menuScreen.classList.remove('hidden');
    this.hudScreen.classList.add('hidden');
    this.winScreen.classList.add('hidden');
  }

  showGame(levelNum: number) {
    this.menuScreen.classList.add('hidden');
    this.hudScreen.classList.remove('hidden');
    this.winScreen.classList.add('hidden');
    this.levelDisplay.textContent = `Level ${levelNum}`;
  }

  showWin() {
    this.winScreen.classList.remove('hidden');
  }

  updateLevel(levelNum: number) {
    this.levelDisplay.textContent = `Level ${levelNum}`;
  }
}
