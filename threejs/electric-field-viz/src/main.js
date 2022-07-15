import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls';
import Stats from 'three/examples/jsm/libs/stats.module.js';

import { createNode, createArrow } from './draw';

// -- Initialization stuff ----------------------------------------------
const renderer = new THREE.WebGLRenderer({antialias: true});
renderer.setClearColor( 0x000000, 1);

renderer.setSize(window.innerWidth, window.innerHeight);
renderer.setPixelRatio( window.devicePixelRatio);

document.body.appendChild(renderer.domElement);
const camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 1200);
camera.position.set(65.30402538438133, 85.91940939655149, 223.8420448517377);
// camera.lookAt(120, 50, 0);

const scene = new THREE.Scene();
const font = await new THREE.FontLoader().loadAsync('fonts/helvetiker_regular.typeface.json');

const stats = new Stats();
stats.showPanel(0); // 0: fps, 1: ms, 2: mb, 3+: custom
document.body.appendChild( stats.dom );

const controls = new OrbitControls( camera, renderer.domElement );
controls.maxPolarAngle = Math.PI * 0.5;
controls.minDistance = 10;
controls.maxDistance = 5000;

// -- Custom Stuff related to scene building -----------------------------

const axesHelper = new THREE.AxesHelper(20);
scene.add(axesHelper);

const radius = 12;
const charge1 = createNode('+', font, radius, new THREE.Vector3(0, 0, 0));
const charge2 = createNode('-', font, radius, new THREE.Vector3(100, 0, 0));
const q = createNode('q', font, radius * 0.5, new THREE.Vector3(50, 120, 0));

console.log(q);

const fieldOfCharge1onQ = new THREE.Vector3(0,0,0)
                                  .subVectors(charge1.position, q.position)
                                  .normalize();

// const fieldOfCharge1onQArrow = createArrow(new THREE.Vector3(0,0,0), fieldOfCharge1onQ, 10);
// scene.add(fieldOfCharge1onQArrow);

// console.log('fieldOfCharge1onQ');
// console.log(fieldOfCharge1onQ);

const fieldOfCharge2onQ = new THREE.Vector3(0,0,0)
                                  .add(charge2.position)
                                  .lerp(new THREE.Vector3(0,0,0).subVectors(q.position, charge2.position), 20)
                                  .normalize();

const fieldOfCharge2onQArrow = createArrow(new THREE.Vector3(0,0,0), fieldOfCharge2onQ, 30);
scene.add(fieldOfCharge2onQArrow);

scene.add(charge1);
scene.add(charge2);
scene.add(q);


// -- ThreeJS Functions --------------------------------------------------
function animate(now){

    requestAnimationFrame(animate);

    controls.update();

    stats.update();
    renderer.render(scene, camera);
}

// render();
animate();

export default function debugCameraPosition(){
    console.log('hey')
}

