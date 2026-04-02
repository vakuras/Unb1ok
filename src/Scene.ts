import * as THREE from 'three';
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';

export class Scene {
  readonly renderer: THREE.WebGLRenderer;
  readonly scene: THREE.Scene;
  readonly camera: THREE.PerspectiveCamera;
  readonly controls: OrbitControls;
  private groundPlane: THREE.Plane;

  constructor(canvas: HTMLCanvasElement) {
    // Renderer
    this.renderer = new THREE.WebGLRenderer({
      canvas,
      antialias: true,
      alpha: false,
    });
    this.renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
    this.renderer.setSize(window.innerWidth, window.innerHeight);
    this.renderer.shadowMap.enabled = true;
    this.renderer.shadowMap.type = THREE.PCFSoftShadowMap;
    this.renderer.toneMapping = THREE.ACESFilmicToneMapping;
    this.renderer.toneMappingExposure = 1.2;

    // Scene (background set in setupEnvironment via skybox)
    this.scene = new THREE.Scene();

    // Camera
    this.camera = new THREE.PerspectiveCamera(
      45,
      window.innerWidth / window.innerHeight,
      0.1,
      100
    );
    this.camera.position.set(0, 8, 8);

    // Controls
    this.controls = new OrbitControls(this.camera, canvas);
    this.controls.target.set(0, 0, 0);
    this.controls.enableDamping = true;
    this.controls.dampingFactor = 0.08;
    this.controls.minDistance = 5;
    this.controls.maxDistance = 20;
    this.controls.maxPolarAngle = Math.PI / 2.2;
    this.controls.minPolarAngle = 0.3;
    this.controls.update();

    // Ground plane for raycasting
    this.groundPlane = new THREE.Plane(new THREE.Vector3(0, 1, 0), 0);

    this.setupLights();
    this.setupEnvironment();

    window.addEventListener('resize', () => this.onResize());
  }

  private setupLights() {
    // Ambient
    const ambient = new THREE.AmbientLight(0x8899bb, 0.6);
    this.scene.add(ambient);

    // Main directional (sun)
    const sun = new THREE.DirectionalLight(0xfff4e6, 1.4);
    sun.position.set(5, 10, 5);
    sun.castShadow = true;
    sun.shadow.mapSize.width = 2048;
    sun.shadow.mapSize.height = 2048;
    sun.shadow.camera.near = 0.5;
    sun.shadow.camera.far = 30;
    sun.shadow.camera.left = -8;
    sun.shadow.camera.right = 8;
    sun.shadow.camera.top = 8;
    sun.shadow.camera.bottom = -8;
    sun.shadow.bias = -0.001;
    sun.shadow.radius = 3;
    this.scene.add(sun);

    // Fill light
    const fill = new THREE.DirectionalLight(0x6688cc, 0.3);
    fill.position.set(-5, 5, -3);
    this.scene.add(fill);

    // Rim light
    const rim = new THREE.DirectionalLight(0xff8866, 0.2);
    rim.position.set(0, 3, -8);
    this.scene.add(rim);
  }

  private setupEnvironment() {
    // Skybox from original game textures
    const loader = new THREE.CubeTextureLoader();
    loader.setPath('tex/');
    const skybox = loader.load([
      'sky0_rt.jpg', 'sky0_lf.jpg',
      'sky0_up.jpg', 'sky0_dn.jpg',
      'sky0_ft.jpg', 'sky0_bk.jpg',
    ]);
    this.scene.background = skybox;
    this.scene.environment = skybox;
  }

  private onResize() {
    this.camera.aspect = window.innerWidth / window.innerHeight;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(window.innerWidth, window.innerHeight);
  }

  render() {
    this.controls.update();
    this.renderer.render(this.scene, this.camera);
  }

  getGroundPlane(): THREE.Plane {
    return this.groundPlane;
  }
}
