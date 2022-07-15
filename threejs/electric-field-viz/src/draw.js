import * as THREE from 'three';
import { LineLoop } from 'three';

export function createCircle(radius, resolution = 32){

    const geometry = new THREE.BufferGeometry();
    const vertices = new Float32Array(resolution * 3);
    for (let sample = 0; sample < resolution; sample++){
        const index = sample * 3;
        let angle = (sample / resolution) * Math.PI * 2;
        vertices[index] = Math.cos(angle) * radius;
        vertices[index+1] = Math.sin(angle) * radius;
        vertices[index+2] = 0;
    }
    geometry.setAttribute('position', new THREE.BufferAttribute(vertices, 3));
    return geometry;
}

export function createArrow(position, vector, length){

    const bodyMaterial = new THREE.LineBasicMaterial({color: 0xff00ff});
    const headMaterial = new THREE.LineBasicMaterial({color: 0x00ff00});

    console.log(position);
    console.log(vector);
    console.log(length);

    const bodyLength = length * 0.75;
    const headLength = length * 0.25;

    const bodyPoints = [
        vector,
        vector.clone().add(new THREE.Vector3(bodyLength,bodyLength,0))
    ];
    const bodyGeo = new THREE.BufferGeometry().setFromPoints(bodyPoints);

    const headPoints = [
        vector.clone().add(new THREE.Vector3(bodyLength,bodyLength,0)),
        vector.clone().add(new THREE.Vector3(bodyLength+headLength,bodyLength+headLength,0))
    ];
    const headGeo = new THREE.BufferGeometry().setFromPoints(headPoints);

    const line = new THREE.Group();
    line.add(new THREE.Line(bodyGeo, bodyMaterial));
    line.add(new THREE.Line(headGeo, headMaterial));

    line.translateX(position.x);
    line.translateY(position.y);
    line.translateZ(position.z);

    return line;
}

export function createNode(text, font, radius, position, useFixedRadius=true){

    const node = new THREE.Group();

    // Add some text in the circle
    const textGeometry = new THREE.TextGeometry(text, {
        font: font,
        size: radius * 0.95,
        height: 0.2,
        curveSegments: 12,
        bevelEnabled: false,
        bevelThickness: 10,
        bevelSize: 8,
        bevelOffset: 0,
        bevelSegments: 5
    });
    textGeometry.computeBoundingBox();

    const midPoint = new THREE.Vector3(
        textGeometry.boundingBox.min.x,
        textGeometry.boundingBox.min.y,
        textGeometry.boundingBox.min.z,
    );
    midPoint.lerp(textGeometry.boundingBox.max, 0.5);

    // Create the node representation
    let circleGeometry;
    if (useFixedRadius){
        circleGeometry = new THREE.CircleGeometry(radius, 32);
    }
    else {
        // circleGeometry = new THREE.CircleGeometry(radius + midPoint.x, 32);
        circleGeometry = new THREE.CircleGeometry(radius, 32);
    }

    const textColor = text === '+' ? 0x11: 0xffffff;
    const circleColor = text === '+' ? 0xffffff : 0x222222;

    // Center the text
    textGeometry.translate(-midPoint.x, -midPoint.y, -midPoint.z);

    node.add(new THREE.Mesh(circleGeometry, new THREE.MeshBasicMaterial({color: circleColor})));
    node.add(new THREE.Mesh(textGeometry, new THREE.MeshBasicMaterial({color: textColor})));

    node.translateX(position.x);
    node.translateY(position.y);
    node.translateZ(position.z);

    return node;
}