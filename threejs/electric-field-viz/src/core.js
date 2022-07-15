const COULOMB_CONSTANT = 9 * 10**-9;

export function computeField(pointPosition, charge1Position, charge1ElectricCharge, charge2Position, charge2ElectricCharge){

    // Find the distances from the charges to the point
    const dist1 = pointPosition.distanceTo(charge1Position);
    const dist2 = pointPosition.distanceTo(charge2Position);

    // Find the unit vector pointing from the charge to the point
    const charge1FieldVector = (charge1Position - pointPosition).normalize();
    const charge2FieldVector = (charge2Position - pointPosition).normalize();

    const charge1FieldMagnitude = COULOMB_CONSTANT * (charge1ElectricCharge / dist1);
    const charge2FieldMagnitude = COULOMB_CONSTANT * (charge2ElectricCharge / dist1);
}

export function debugCameraPosition(){
    console.log('hey')
}

