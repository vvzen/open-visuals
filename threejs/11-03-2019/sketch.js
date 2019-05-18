//----------------- TITLE -----------------
//             Social Pressure
//-----------------------------------------

const currentId = 1002;
const NUM_AGENTS = 64;

let currentIntersection = null;
let container;
let camera, scene, renderer;
// let raycaster;
// let mouseMesh;
let mouse = new THREE.Vector2(0, 0);
let pointLight, keyLight;

let targetMesh;

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
    // if (mouseMesh) {
    //     mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
    //     mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
    // }
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
function init() {

    console.log('init()');

    // raycaster = new THREE.Raycaster();

    container = document.getElementById('container');

    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 10000);
    camera.position.set(540, 200, 1200);

    // let aspect = window.innerWidth / window.innerHeight;
    // camera = new THREE.OrthographicCamera(FRUSTUM_SIZE * aspect / -2, FRUSTUM_SIZE * aspect / 2, FRUSTUM_SIZE / 2, FRUSTUM_SIZE / -2, 1, 1000);

    scene = new THREE.Scene();
    // scene.fog = new THREE.FogExp2(0x000000, 0.0008);

    let targetGeo = new THREE.SphereBufferGeometry(20, 16, 16);
    targetMesh = new THREE.Mesh(targetGeo, new THREE.MeshBasicMaterial({
        color: 0xfffffff
    }));
    scene.add(targetMesh);

    // SPAWN AGENTS
    for (let i = 0.0; i < Math.PI * 2; i += Math.PI / NUM_AGENTS) {

        // let currentAgent = new Agent(new THREE.Vector3(0, 0, 0));
        // let currentGeo;
        // if (Math.random() > 0.5) {
        //     currentGeo = new THREE.CylinderBufferGeometry(Math.random() * 4, 10, 100, 12);
        //     currentGeo.rotateX(Math.PI / 2);
        // } else {
        // }
        let currentGeo = new THREE.BoxBufferGeometry(10, 10, 100);

        let color = new THREE.Color();
        color.setHSL(100, 90, 100);

        currentMaterial = new THREE.MeshLambertMaterial({
            color: color
        });

        let currentMesh = new THREE.Mesh(currentGeo, currentMaterial);

        currentMesh.position.x = targetMesh.position.x + Math.sin(i) * 400;
        currentMesh.position.y += THREE.Math.mapLinear(Math.random(), 0, 1, -150, 150);
        currentMesh.position.z = targetMesh.position.z + Math.cos(i) * 400;

        let scaleFactor = THREE.Math.mapLinear(Math.random(), 0, 1, 0.5, 1.5);
        currentMesh.scale.x *= scaleFactor;
        currentMesh.scale.y *= scaleFactor;
        currentMesh.scale.z *= scaleFactor;

        agentsMeshes.push(currentMesh);
        scene.add(currentMesh);
    }

    // let mouseGeo = new THREE.SphereBufferGeometry(16);
    // mouseMesh = new THREE.Mesh(mouseGeo, new THREE.MeshBasicMaterial({
    //     color: 0x00ff00
    // }));
    // scene.add(mouseMesh);

    // let helper = new THREE.GridHelper(1000, 10);
    // scene.add(helper);

    // LIGHTS
    pointLight = new THREE.PointLight(0xffffff, 1);
    scene.add(pointLight);

    keyLight = new THREE.DirectionalLight(0xffffff, 0.625);
    keyLight.position.x -= 1;
    keyLight.position.z += 0.9;
    scene.add(keyLight);

    let hemiLight = new THREE.HemisphereLight(0xffffff, 0xffffff, 0.3);
    hemiLight.color.setHSL(0.6, 1, 0.6);
    hemiLight.groundColor.setHSL(0.095, 1, 0.75);
    hemiLight.position.set(0, 50, 0);
    scene.add(hemiLight);

    let ambientLight = new THREE.AmbientLight(new THREE.Color().setHSL(0.7, 0.6, 0.4));
    scene.add(ambientLight);


    renderer = new THREE.WebGLRenderer({
        antialias: true
    });
    renderer.setClearColor(0x111111);
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

    // var time = Date.now() * 0.0005;

    targetMesh.position.x = Math.sin(timestamp * 0.001) * 225;
    targetMesh.position.y = Math.cos(timestamp * 0.003) * 100;
    targetMesh.position.z = Math.sin(timestamp * 0.003) * 300;

    render();
}

//---------------- RENDER -----------------
function render() {

    // raycaster.setFromCamera(mouse, camera);
    // let intersections = raycaster.intersectObjects(agentsMeshes);
    // currentIntersection = (intersections.length) > 0 ? intersections[0] : null;

    // if (intersections.length > 0) {
    //     currentIntersection = intersections[0];
    // }

    // if (currentIntersection !== null) {
    //     mouseMesh.position.copy(currentIntersection.point);
    // }

    // let test = new THREE.Vector3(mouse.x, mouse.y, 0.5);
    // let target = test.unproject(camera);

    // mouseMesh.position.copy(new THREE.Vector3(mouse.x, mouse.y, 0));

    agentsMeshes.forEach((mesh, i) => {
        mesh.lookAt(targetMesh.position);

        let newColor = new THREE.Color();
        let distance = THREE.Math.mapLinear(mesh.position.distanceTo(targetMesh.position), 0, 1000, 1.0, 0.0);
        // mesh.material.color.setHex(0xff00ff);
        mesh.material.color.setHSL(distance, distance, 0.6);
    });

    // camera.lookAt(targetMesh.position);

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
