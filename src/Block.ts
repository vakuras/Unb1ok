import * as THREE from 'three';
import { CELL, HALF, BOARD_Y, GRID } from './Board';

const BLOCK_HEIGHT = 0.4;
const ROUND_SEGMENTS = 2;

// Colors
const HERO_COLOR = 0xff6b4a;
const HERO_EMISSIVE = 0x441100;
const NORMAL_COLORS = [0x5588aa, 0x667799, 0x557788, 0x6688aa, 0x778899];
const HOVER_EMISSIVE = 0x222244;

export class Block {
  readonly mesh: THREE.Mesh;
  readonly isHero: boolean;
  readonly xDirection: boolean;
  readonly gridW: number;
  readonly gridD: number;

  // Grid position (in grid coords 0-5)
  gridX: number;
  gridY: number;

  // Animation
  private targetX: number;
  private targetZ: number;
  private baseMaterial: THREE.MeshStandardMaterial;

  constructor(
    gridX: number, gridY: number,
    gridW: number, gridD: number,
    dir: number, index: number
  ) {
    this.isHero = index === 0;
    this.xDirection = dir === 0;
    this.gridX = gridX;
    this.gridY = gridY;
    this.gridW = gridW;
    this.gridD = gridD;

    // World dimensions
    const worldW = gridW * CELL;
    const worldD = gridD * CELL;

    // Geometry with slight bevel
    const geo = new THREE.BoxGeometry(
      worldW - 0.06, BLOCK_HEIGHT, worldD - 0.06,
      ROUND_SEGMENTS, ROUND_SEGMENTS, ROUND_SEGMENTS
    );

    // Material
    const color = this.isHero
      ? HERO_COLOR
      : NORMAL_COLORS[index % NORMAL_COLORS.length];

    this.baseMaterial = new THREE.MeshStandardMaterial({
      color,
      roughness: this.isHero ? 0.3 : 0.5,
      metalness: this.isHero ? 0.4 : 0.2,
      emissive: this.isHero ? HERO_EMISSIVE : 0x000000,
      emissiveIntensity: this.isHero ? 0.3 : 0,
      envMapIntensity: 0.8,
    });

    this.mesh = new THREE.Mesh(geo, this.baseMaterial);
    this.mesh.castShadow = true;
    this.mesh.receiveShadow = true;
    this.mesh.userData.block = this;

    // Position
    const worldPos = this.gridToWorld(gridX, gridY);
    this.mesh.position.set(worldPos.x, BOARD_Y + BLOCK_HEIGHT / 2, worldPos.z);
    this.targetX = this.mesh.position.x;
    this.targetZ = this.mesh.position.z;
  }

  /** Convert grid coords to world center position */
  gridToWorld(gx: number, gy: number): { x: number; z: number } {
    return {
      x: -HALF + gx * CELL + (this.gridW * CELL) / 2,
      z: -HALF + gy * CELL + (this.gridD * CELL) / 2,
    };
  }

  /** Convert world position to grid coords (top-left corner) */
  worldToGrid(wx: number, wz: number): { gx: number; gy: number } {
    return {
      gx: (wx - (this.gridW * CELL) / 2 + HALF) / CELL,
      gy: (wz - (this.gridD * CELL) / 2 + HALF) / CELL,
    };
  }

  setHover(hover: boolean) {
    if (hover) {
      this.baseMaterial.emissive.setHex(
        this.isHero ? 0x662200 : HOVER_EMISSIVE
      );
      this.baseMaterial.emissiveIntensity = 0.5;
    } else {
      this.baseMaterial.emissive.setHex(
        this.isHero ? HERO_EMISSIVE : 0x000000
      );
      this.baseMaterial.emissiveIntensity = this.isHero ? 0.3 : 0;
    }
  }

  setDragging(dragging: boolean) {
    if (dragging) {
      this.mesh.position.y = BOARD_Y + BLOCK_HEIGHT / 2 + 0.1;
      this.baseMaterial.emissiveIntensity = 0.6;
    } else {
      this.mesh.position.y = BOARD_Y + BLOCK_HEIGHT / 2;
      this.setHover(false);
    }
  }

  /** Try to move to a world position, clamping to axis and board bounds */
  moveTo(
    worldX: number, worldZ: number,
    blocks: Block[], myIndex: number
  ): { moved: boolean; tableHit: boolean; blockHit: boolean } {
    let newX = this.mesh.position.x;
    let newZ = this.mesh.position.z;
    let tableHit = false;
    let blockHit = false;

    const halfW = (this.gridW * CELL) / 2;
    const halfD = (this.gridD * CELL) / 2;

    if (this.xDirection) {
      newX = worldX;
      // Clamp to board (left side only for hero, both sides for others)
      const minX = -HALF + halfW;
      if (newX < minX) { newX = minX; tableHit = true; }
      if (!this.isHero) {
        const maxX = HALF - halfW;
        if (newX > maxX) { newX = maxX; tableHit = true; }
      }
    } else {
      newZ = worldZ;
      const minZ = -HALF + halfD;
      const maxZ = HALF - halfD;
      if (newZ < minZ) { newZ = minZ; tableHit = true; }
      if (newZ > maxZ) { newZ = maxZ; tableHit = true; }
    }

    // Check collisions with other blocks
    for (let i = 0; i < blocks.length; i++) {
      if (i === myIndex) continue;
      const other = blocks[i];
      if (this.overlaps(newX, newZ, other)) {
        blockHit = true;
        // Push back to just touching
        if (this.xDirection) {
          const otherLeft = other.mesh.position.x - (other.gridW * CELL) / 2;
          const otherRight = other.mesh.position.x + (other.gridW * CELL) / 2;
          if (newX > this.mesh.position.x) {
            newX = otherLeft - halfW;
          } else {
            newX = otherRight + halfW;
          }
        } else {
          const otherTop = other.mesh.position.z - (other.gridD * CELL) / 2;
          const otherBottom = other.mesh.position.z + (other.gridD * CELL) / 2;
          if (newZ > this.mesh.position.z) {
            newZ = otherTop - halfD;
          } else {
            newZ = otherBottom + halfD;
          }
        }
      }
    }

    const moved = newX !== this.mesh.position.x || newZ !== this.mesh.position.z;
    this.mesh.position.x = newX;
    this.mesh.position.z = newZ;
    this.targetX = newX;
    this.targetZ = newZ;

    return { moved, tableHit, blockHit };
  }

  /** Snap to nearest grid position */
  snapToGrid() {
    const grid = this.worldToGrid(this.mesh.position.x, this.mesh.position.z);
    const snappedGx = Math.round(grid.gx * 2) / 2; // snap to half-grid
    const snappedGy = Math.round(grid.gy * 2) / 2;
    const worldPos = this.gridToWorld(snappedGx, snappedGy);
    this.targetX = worldPos.x;
    this.targetZ = worldPos.z;
    this.gridX = snappedGx;
    this.gridY = snappedGy;
  }

  /** Check if hero block has exited the board */
  hasExited(): boolean {
    return this.isHero && this.mesh.position.x > HALF + 0.5;
  }

  /** AABB overlap test */
  private overlaps(myX: number, myZ: number, other: Block): boolean {
    const margin = 0.05;
    const myHalfW = (this.gridW * CELL) / 2 - margin;
    const myHalfD = (this.gridD * CELL) / 2 - margin;
    const otherHalfW = (other.gridW * CELL) / 2 - margin;
    const otherHalfD = (other.gridD * CELL) / 2 - margin;

    return (
      myX - myHalfW < other.mesh.position.x + otherHalfW &&
      myX + myHalfW > other.mesh.position.x - otherHalfW &&
      myZ - myHalfD < other.mesh.position.z + otherHalfD &&
      myZ + myHalfD > other.mesh.position.z - otherHalfD
    );
  }

  /** Animate toward target (call in update loop) */
  update(dt: number) {
    const speed = 15;
    this.mesh.position.x += (this.targetX - this.mesh.position.x) * Math.min(1, speed * dt);
    this.mesh.position.z += (this.targetZ - this.mesh.position.z) * Math.min(1, speed * dt);
  }

  dispose() {
    this.mesh.geometry.dispose();
    (this.mesh.material as THREE.Material).dispose();
  }
}
