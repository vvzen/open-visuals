const PLANE_SUBDVISIONS = 32;
const FRUSTUM_SIZE = 1000;
const currentId = 1001;

let currentIntersection = null;
let container;
let camera, scene, renderer, focusPoint;
let raycaster;
let mouseMesh;
let mouse = new THREE.Vector2(0, 0);
let pointLight, keyLight;

let agentsMeshes = [];

//--------------- CALLBACKS ---------------
function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
}

function onMouseMove(event) {

    event.preventDefault();

    // map mouse to threejs coordinate system
    if (mouseMesh) {
        // mouseMesh.translateOnAxis(new THREE.Vector3(0, 1, 0), 10);
        mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
        mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
    }
}

//------------------ GUI ------------------
let guiParameters = {
    "light_x_pos": 0,
    "light_y_pos": 0,
    "light_z_pos": 0,
    "focus_point_y": 0
}

function addGui() {

    let gui = new dat.GUI();
    gui.remember(guiParameters);

    gui.add(guiParameters, 'light_x_pos', -1.0, 1.0, 0.0001).onChange(() => {
        keyLight.position.x = guiParameters.light_x_pos;
    });
}

//----------------- SETUP -----------------
function init() {

    console.log('init()');

    raycaster = new THREE.Raycaster();

    container = document.getElementById('container');

    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 10000);
    camera.position.set(540, 400, 1200);

    // let aspect = window.innerWidth / window.innerHeight;
    // camera = new THREE.OrthographicCamera(FRUSTUM_SIZE * aspect / -2, FRUSTUM_SIZE * aspect / 2, FRUSTUM_SIZE / 2, FRUSTUM_SIZE / -2, 1, 1000);

    scene = new THREE.Scene();
    // scene.fog = new THREE.FogExp2(0x000000, 0.0008);

    // SPAWN AGENTS
    for (let i = 0; i < 1; i++) {

        let currentAgent = new Agent(new THREE.Vector3(0, 0, 0));
        let currentGeo = new THREE.BoxBufferGeometry(200, 50, 200);

        let color = new THREE.Color(0xff0000);

        let currentMesh = new THREE.Mesh(currentGeo, new THREE.MeshLambertMaterial({
            color: color
        }));
        // currentMesh.position.x -= 200;

        agentsMeshes.push(currentMesh);
        scene.add(currentMesh);
    }

    let mouseGeo = new THREE.SphereBufferGeometry(16);
    mouseMesh = new THREE.Mesh(mouseGeo, new THREE.MeshBasicMaterial({
        color: 0x00ff00
    }));
    scene.add(mouseMesh);

    let helper = new THREE.GridHelper(1000, 10);
    scene.add(helper);

    // LIGHTS
    pointLight = new THREE.PointLight(0xffffff, 1);
    keyLight = new THREE.DirectionalLight(0xffffff, 0.625);
    keyLight.position.x -= 1;
    keyLight.position.z += 0.9;

    let hemiLight = new THREE.HemisphereLight(0xffffff, 0xffffff, 0.3);
    hemiLight.color.setHSL(0.6, 1, 0.6);
    hemiLight.groundColor.setHSL(0.095, 1, 0.75);
    hemiLight.position.set(0, 50, 0);
    scene.add(hemiLight);

    let ambientLight = new THREE.AmbientLight(new THREE.Color().setHSL(0.7, 0.6, 0.4));
    scene.add(ambientLight);

    // scene.add(pointLight);
    scene.add(keyLight);

    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setPixelRatio(window.devicePixelRatio);
    container.appendChild(renderer.domElement);

    var controls = new THREE.OrbitControls(camera, renderer.domElement);

    // GUI
    // addGui();

    onWindowResize();

    window.addEventListener('resize', onWindowResize, false);
}

//---------------- UPDATE -----------------
function animate(timestamp) {

    requestAnimationFrame(animate);

    // (agentsMeshes).forEach((mesh, i) => {
    //     let frequency = 0.001;
    //     mesh.position.y = THREE.Math.mapLinear(Math.sin((i * 10) + timestamp * frequency), -1, 1, -400, 400);
    // });

    render();
}

//---------------- RENDER -----------------
function render() {

    raycaster.setFromCamera(mouse, camera);
    let intersections = raycaster.intersectObjects(agentsMeshes);
    currentIntersection = (intersections.length) > 0 ? intersections[0] : null;

    // if (intersections.length > 0) {
    //     currentIntersection = intersections[0];
    // }

    if (currentIntersection !== null) {
        mouseMesh.position.copy(currentIntersection.point);
    }

    renderer.render(scene, camera);
}

//-------------- ENTRY POINT --------------
if (WEBGL.isWebGLAvailable() === false) {
    document.body.appendChild(WEBGL.getWebGLErrorMessage());
}

document.addEventListener('mousemove', onMouseMove, false);

console.log(`current session is ${currentId}`);

init();
animate();