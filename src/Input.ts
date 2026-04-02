import * as THREE from 'three';
import { Block } from './Block';
import { Scene as GameScene } from './Scene';

export class Input {
  private raycaster = new THREE.Raycaster();
  private mouse = new THREE.Vector2();
  private dragPlane = new THREE.Plane(new THREE.Vector3(0, 1, 0), 0);
  private dragOffset = new THREE.Vector3();
  private intersection = new THREE.Vector3();

  private hoveredBlock: Block | null = null;
  private draggedBlock: Block | null = null;
  private draggedIndex = -1;
  private isDragging = false;

  private gameScene: GameScene;
  private getBlocks: () => Block[];

  onWin: (() => void) | null = null;
  onSlide: (() => void) | null = null;

  constructor(gameScene: GameScene, getBlocks: () => Block[]) {
    this.gameScene = gameScene;
    this.getBlocks = getBlocks;

    const canvas = gameScene.renderer.domElement;
    canvas.addEventListener('pointerdown', (e) => this.onPointerDown(e));
    canvas.addEventListener('pointermove', (e) => this.onPointerMove(e));
    canvas.addEventListener('pointerup', () => this.onPointerUp());
    canvas.addEventListener('pointerleave', () => this.onPointerUp());
  }

  private updateMouse(e: PointerEvent) {
    const rect = this.gameScene.renderer.domElement.getBoundingClientRect();
    this.mouse.x = ((e.clientX - rect.left) / rect.width) * 2 - 1;
    this.mouse.y = -((e.clientY - rect.top) / rect.height) * 2 + 1;
  }

  private pickBlock(): { block: Block; index: number } | null {
    this.raycaster.setFromCamera(this.mouse, this.gameScene.camera);
    const blocks = this.getBlocks();
    const meshes = blocks.map((b) => b.mesh);
    const hits = this.raycaster.intersectObjects(meshes, false);

    if (hits.length > 0) {
      const block = hits[0].object.userData.block as Block;
      const index = blocks.indexOf(block);
      return { block, index };
    }
    return null;
  }

  private getPlaneIntersection(): THREE.Vector3 | null {
    this.raycaster.setFromCamera(this.mouse, this.gameScene.camera);
    const target = new THREE.Vector3();
    const result = this.raycaster.ray.intersectPlane(this.dragPlane, target);
    return result;
  }

  private onPointerDown(e: PointerEvent) {
    this.updateMouse(e);
    const pick = this.pickBlock();

    if (pick) {
      this.draggedBlock = pick.block;
      this.draggedIndex = pick.index;
      this.isDragging = true;

      // Disable orbit controls while dragging
      this.gameScene.controls.enabled = false;

      // Set drag plane at block height
      this.dragPlane.set(
        new THREE.Vector3(0, 1, 0),
        -pick.block.mesh.position.y
      );

      // Calculate offset from block center to click point
      const planePoint = this.getPlaneIntersection();
      if (planePoint) {
        this.dragOffset.copy(pick.block.mesh.position).sub(planePoint);
      }

      pick.block.setDragging(true);
    }
  }

  private onPointerMove(e: PointerEvent) {
    this.updateMouse(e);

    if (this.isDragging && this.draggedBlock) {
      const planePoint = this.getPlaneIntersection();
      if (planePoint) {
        const targetX = planePoint.x + this.dragOffset.x;
        const targetZ = planePoint.z + this.dragOffset.z;

        const blocks = this.getBlocks();
        const result = this.draggedBlock.moveTo(
          targetX, targetZ, blocks, this.draggedIndex
        );

        // Play slide sound on collision
        if (result.blockHit || result.tableHit) {
          this.onSlide?.();
        }

        // Check win
        if (this.draggedBlock.hasExited() && this.onWin) {
          this.onPointerUp();
          this.onWin();
        }
      }
    } else {
      // Hover detection
      const pick = this.pickBlock();
      if (pick && pick.block !== this.hoveredBlock) {
        if (this.hoveredBlock) this.hoveredBlock.setHover(false);
        this.hoveredBlock = pick.block;
        pick.block.setHover(true);
        this.gameScene.renderer.domElement.style.cursor = 'grab';
      } else if (!pick && this.hoveredBlock) {
        this.hoveredBlock.setHover(false);
        this.hoveredBlock = null;
        this.gameScene.renderer.domElement.style.cursor = 'default';
      }
    }
  }

  private onPointerUp() {
    if (this.draggedBlock) {
      this.draggedBlock.snapToGrid();
      this.draggedBlock.setDragging(false);
      this.draggedBlock = null;
      this.draggedIndex = -1;
    }
    this.isDragging = false;
    this.gameScene.controls.enabled = true;
    this.gameScene.renderer.domElement.style.cursor = 'default';
  }

  get dragging(): boolean {
    return this.isDragging;
  }
}
