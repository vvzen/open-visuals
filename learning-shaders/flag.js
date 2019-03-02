var container;

var camera, scene, renderer;

var uniforms;

let pointLight;
let flagMaterial;

const PLANE_SUBDVISIONS = 32;

let fragmentShader = `
        uniform sampler2D texture0;
        uniform float time;

        varying vec2 vUv;

        void main(){
            
            vec4 tex=texture2D(texture0,vUv);
            vec4 newTex=vec4(tex.r,(tex.g+tex.b)*.5,(tex.g+tex.b)*.5,1.);
            
            //gl_FragColor = vec4(vUv.x, vUv.y, 0.0, 1.0);
            gl_FragColor=newTex;
            
        }`;

// Async loading of the shaders
fetch('src/gl/frag.glsl')
    .then(response => response.text())
    .then((resolve, reject) => {

        fragmentShader = resolve;

        init();
        animate();
    })
    .catch(error => {
        console.log(error)
    });


if (WEBGL.isWebGLAvailable() === false) {

    document.body.appendChild(WEBGL.getWebGLErrorMessage());

}

function init() {

    container = document.getElementById('container');

    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 4000);
    camera.position.set(540, 700, 1200);

    scene = new THREE.Scene();

    // console.log(scene.position);

    // TEXTURES
    let texture = new THREE.TextureLoader().load("kermit.jpg");
    texture.minFilter = THREE.LinearFilter;
    texture.magFilter = THREE.NearestFilter;

    uniforms = {
        "time": {
            type: "float",
            value: 1.0
        },
        texture0: {
            value: texture

        },
        "x_frequency": {
            type: "float",
            value: 4.5
        },
        "y_frequency": {
            type: "float",
            value: 3.5
        },
        "displacement": {
            type: "float",
            value: 10.0
        }
    };

    // MATERIALS
    var phongMaterial = new THREE.MeshPhongMaterial({
        color: 0xdddddd,
        specular: 0xaaaaaa,
        emissive: 0x000000,
        shininess: 0,
        opacity: 1.0
    });

    flagMaterial = new THREE.ShaderMaterial({
        uniforms: uniforms,
        vertexShader: document.getElementById('vertexShader').textContent,
        fragmentShader: fragmentShader
    });

    var wireframeMaterial = new THREE.MeshBasicMaterial({
        color: 0xff00000,
        wireframe: true,
        opacity: 1.0,
        transparent: false
    });

    // GEO
    let flagGeo = new THREE.PlaneBufferGeometry(400, 400, PLANE_SUBDVISIONS, PLANE_SUBDVISIONS);
    flagGeo.translate(0.0, 600.0, 0.0);
    let flagMesh = new THREE.Mesh(flagGeo, flagMaterial);
    scene.add(flagMesh);

    var poleGeo = new THREE.BoxGeometry(10, 800, 10)
    poleGeo.translate(-190, 400, 0);
    console.log(flagGeo);
    var poleMesh = new THREE.Mesh(poleGeo, phongMaterial)
    scene.add(poleMesh);

    var helper = new THREE.GridHelper(1000, 10);
    scene.add(helper);

    // LIGHTS
    pointLight = new THREE.PointLight(0xffffff, 1);
    var directionalLight = new THREE.DirectionalLight(0xffffff, 0.725);
    directionalLight.position.x -= 1;
    directionalLight.position.z += 0.9;

    let ambientLight = new THREE.AmbientLight(0xffffff);
    scene.add(ambientLight);

    // scene.add(pointLight);
    // scene.add(directionalLight);

    renderer = new THREE.WebGLRenderer();
    renderer.setPixelRatio(window.devicePixelRatio);
    container.appendChild(renderer.domElement);

    var controls = new THREE.OrbitControls(camera, renderer.domElement);

    // GUI
    let guiParameters = {
        "x_frequency": 1.5,
        "y_frequency": 4.5,
        "displacement": 20,
    }

    let gui = new dat.GUI();
    gui.remember(guiParameters);
    //parameters, 'inclination', 0, 0.5, 0.0001

    gui.add(guiParameters, 'x_frequency', 0.0, 1.0, 0.0001).onChange(() => {
        flagMaterial.uniforms["x_frequency"].value = guiParameters.x_frequency;
    });
    gui.add(guiParameters, 'y_frequency', 0.0, 0.4, 0.0001).onChange(() => {
        flagMaterial.uniforms["y_frequency"].value = guiParameters.y_frequency;
    });
    gui.add(guiParameters, 'displacement', 0.0, 50, 0.1).onChange(() => {
        flagMaterial.uniforms["displacement"].value = guiParameters.displacement;
    });

    onWindowResize();

    window.addEventListener('resize', onWindowResize, false);

}

function onWindowResize() {

    renderer.setSize(window.innerWidth, window.innerHeight);

}

function animate(timestamp) {

    requestAnimationFrame(animate);

    uniforms["time"].value = timestamp / 250.0;

    render();
}

function render() {

    // let cameraFocus = scene.position;
    // cameraFocus.y -= 10;
    camera.lookAt(new THREE.Vector3(20, 350, 0));
    // console.log("time:");
    // console.log(uniforms["time"].value);

    renderer.render(scene, camera);
}