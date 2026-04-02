import * as THREE from 'three';
import { LEVELS, type LevelData } from './levels';
import { Block } from './Block';

export class Level {
  blocks: Block[] = [];
  currentIndex = 0;

  get totalLevels(): number {
    return LEVELS.length;
  }

  load(index: number, scene: THREE.Scene): boolean {
    this.unload(scene);

    if (index < 0 || index >= LEVELS.length) return false;
    this.currentIndex = index;

    const data: LevelData = LEVELS[index];
    this.blocks = data.map((b, i) =>
      new Block(b.x, b.y, b.w, b.d, b.dir, i)
    );

    for (const block of this.blocks) {
      scene.add(block.mesh);
    }

    return true;
  }

  unload(scene: THREE.Scene) {
    for (const block of this.blocks) {
      scene.remove(block.mesh);
      block.dispose();
    }
    this.blocks = [];
  }

  nextLevel(scene: THREE.Scene): boolean {
    return this.load(this.currentIndex + 1, scene);
  }

  prevLevel(scene: THREE.Scene): boolean {
    return this.load(Math.max(0, this.currentIndex - 1), scene);
  }

  update(dt: number) {
    for (const block of this.blocks) {
      block.update(dt);
    }
  }

  checkWin(): boolean {
    return this.blocks.length > 0 && this.blocks[0].hasExited();
  }
}
