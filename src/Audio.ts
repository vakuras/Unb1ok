export class Audio {
  private ctx: AudioContext | null = null;
  private enabled = true;
  private buffers: Map<string, AudioBuffer> = new Map();
  private lastSlideTime = 0;

  constructor() {
    try {
      this.ctx = new AudioContext();
      this.preload();
    } catch {
      this.enabled = false;
    }
  }

  private async preload() {
    const files = ['hover', 'select', 'finito', 'pad1', 'pad2', 'intro'];
    for (const name of files) {
      try {
        const resp = await fetch(`tex/${name}.ogg`);
        const data = await resp.arrayBuffer();
        const buffer = await this.ctx!.decodeAudioData(data);
        this.buffers.set(name, buffer);
      } catch { /* ignore missing files */ }
    }
  }

  private ensureResumed() {
    if (this.ctx?.state === 'suspended') {
      this.ctx.resume();
    }
  }

  private playBuffer(name: string, volume = 0.3) {
    if (!this.enabled || !this.ctx) return;
    this.ensureResumed();
    const buffer = this.buffers.get(name);
    if (!buffer) return;

    const source = this.ctx.createBufferSource();
    const gain = this.ctx.createGain();
    source.buffer = buffer;
    gain.gain.value = volume;
    source.connect(gain);
    gain.connect(this.ctx.destination);
    source.start();
  }

  private playTone(freq: number, duration: number, type: OscillatorType = 'sine', volume = 0.15) {
    if (!this.enabled || !this.ctx) return;
    this.ensureResumed();

    const osc = this.ctx.createOscillator();
    const gain = this.ctx.createGain();

    osc.type = type;
    osc.frequency.setValueAtTime(freq, this.ctx.currentTime);
    gain.gain.setValueAtTime(volume, this.ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, this.ctx.currentTime + duration);

    osc.connect(gain);
    gain.connect(this.ctx.destination);
    osc.start();
    osc.stop(this.ctx.currentTime + duration);
  }

  hover() {
    this.playBuffer('hover', 0.2);
  }

  select() {
    this.playBuffer('select', 0.3);
  }

  slide() {
    const now = performance.now();
    if (now - this.lastSlideTime < 150) return; // cooldown
    this.lastSlideTime = now;
    this.playBuffer('pad1', 0.15);
  }

  win() {
    this.playBuffer('finito', 0.4);
  }

  setEnabled(on: boolean) {
    this.enabled = on;
  }
}
