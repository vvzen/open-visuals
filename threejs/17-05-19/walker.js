class Walker {
    constructor(material) {


        this.size = THREE.Math.mapLinear(Math.random(), 0, 1, 5, 45);
        this.geo = new THREE.SphereBufferGeometry(this.size, 16, 16);

        this.color = new THREE.Color();
        this.color.setHSL(THREE.Math.mapLinear(Math.random(), 0, 1, 0.6, 0.96), 0.9, 0.6);
        this.mat = new THREE.MeshLambertMaterial({
            color: this.color,
            transparent: true,
            opacity: 0.9
        });

        this.mesh = new THREE.Mesh(this.geo, this.mat);

        this.acceleration = new THREE.Vector3(0, 0, 0);
        this.velocity = new THREE.Vector3(0, 0, 0);
        this.mesh.position.x = 0;
        this.mesh.position.y = 0;
        this.mesh.position.z = 0;
    }

    update(time) {
        if (time % 5 == 1) {
            let randomAccX = THREE.Math.mapLinear(Math.random(), 0, 1, -10, 10);
            let randomAccY = THREE.Math.mapLinear(Math.random(), 0, 1, -10, 10);
            let randomAccZ = THREE.Math.mapLinear(Math.random(), 0, 1, -10, 10);
            this.acceleration = new THREE.Vector3(randomAccX, randomAccY, randomAccZ);
            this.acceleration.divideScalar(this.size);
            // this.acceleration.normalize();
        } else {
            let target = new THREE.Vector3(0, 0, 0);
            this.acceleration = target.sub(this.mesh.position);
            this.acceleration.multiplyScalar(0.10);
        }

        this.acceleration.normalize();
        this.velocity.add(this.acceleration);
        this.mesh.position.add(this.velocity);
    }
}