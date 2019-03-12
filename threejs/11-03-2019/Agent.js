// class Agent {


//     constructor() {
//         console.log(`hello world from agent ${THREE.Math.generateUUID()}`);
//         console.log(`agent pos: ${this.pos}`);
//     }
//     move() {}
// }

function Agent(position) {

    this.acc = new THREE.Vector3(0, 0, 0);
    this.vel = new THREE.Vector3(0, 0, 0);
    this.pos = new THREE.Vector3(position);
}

Agent.prototype.move = function() {

}