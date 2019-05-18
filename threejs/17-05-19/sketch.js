//----------------- TITLE -----------------
//                 Agents
//-----------------------------------------


//---------------- COSTANTS ---------------
const currentId = 1003;

const SCREEN_WIDTH = window.innerWidth;
const SCREEN_HEIGHT = window.innerHeight;
const aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;

let container;
let camera, scene, renderer;

let pointLight, keyLight;

let walkers = [];

//--------------- CALLBACKS ---------------
function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
}

function onMouseMove(event) {

    event.preventDefault();

    // map mouse to threejs coordinate system
    mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
    mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
}

//------------------ GUI ------------------
let guiParameters = {
    "light_x_pos": 0,
    "light_y_pos": 0,
    "light_z_pos": 0
}

function addGui() {

    let gui = new dat.GUI();
    gui.remember(guiParameters);

    gui.add(guiParameters, 'light_x_pos', -1.0, 1.0, 0.0001).onChange(() => {
        keyLight.position.x = guiParameters.light_x_pos;
    });
}

//----------------- SETUP -----------------
function addLights(scene) {

    let pointLightColor = new THREE.Color();
    pointLightColor.setHSL(0.4, 0., 0.4);

    pointLight = new THREE.PointLight(0xe3904e, 0.8);
    pointLight.position.set(0, 0, 0);
    scene.add(pointLight);

    let hemiLight = new THREE.HemisphereLight(0xffffff, 0xffffff, 0.47);
    hemiLight.color.setHSL(0.6, 1, 0.6);
    hemiLight.groundColor.setHSL(0.095, 1, 0.75);
    hemiLight.position.set(0, 50, 0);
    scene.add(hemiLight);
}


function init() {

    console.log('init()');

    container = document.getElementById('container');

    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 10000);

    camera.position.set(240, 1000, 1000);

    scene = new THREE.Scene();

    for (let i = 0; i < 64; i++) {
        walkers.push(new Walker());
    }
    for (let walker of walkers) {
        console.log(walker);
        scene.add(walker.mesh);
    }

    // LIGHTS
    addLights(scene);

    // RENDERER
    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setClearColor(0x111111);
    renderer.setPixelRatio(window.devicePixelRatio);
    container.appendChild(renderer.domElement);

    let controls = new THREE.OrbitControls(camera, renderer.domElement);

    // let grid = new THREE.GridHelper(1000, 10);
    // scene.add(grid);

    // GUI
    // addGui();

    onWindowResize();
    window.addEventListener('resize', onWindowResize, false);
}

//---------------- UPDATE -----------------
function animate(timestamp) {

    requestAnimationFrame(animate);

    for (let walker of walkers) {
        walker.update(timestamp);
    }

    render();
}

//---------------- RENDER -----------------
function render() {

    renderer.render(scene, camera);
}

//-------------- ENTRY POINT --------------
if (WEBGL.isWebGLAvailable() === false) {
    document.body.appendChild(WEBGL.getWebGLErrorMessage());
}

// document.addEventListener('mousemove', onMouseMove, false);

console.log(`current session is ${currentId}`);

init();
animate();