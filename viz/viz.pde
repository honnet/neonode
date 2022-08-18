import processing.serial.*;

String receivedString = null;
Serial myPort;  // The serial port

float[] xyz = {0,0,0};
float[] xy_max = {1260, 1043};

boolean use_trail = false;


void setup() {
//    fullScreen();
    size(1280, 720);
    strokeWeight(5);
    background(0);

    // List all the available serial ports
    printArray(Serial.list());
    myPort = new Serial(this, Serial.list()[0], 11500);
    myPort.clear();
    // Throw out the first reading, in case we started reading
    // in the middle of a string from the sender.
    receivedString = myPort.readStringUntil('\n');
    receivedString = null;
    println("\nStarting: touch the device, it might up to 10 seconds...");
}

void draw() {
    while (myPort.available() > 0) {

        receivedString = myPort.readStringUntil('\n');
        println(receivedString);

        if (receivedString != null &&
            receivedString.length() == 12 &&
            receivedString.charAt(4) == '.')
        {
            xyz[0] = Float.parseFloat(receivedString.substring(0, 4));
            xyz[1] = Float.parseFloat(receivedString.substring(5, 9));
            xyz[2] = Float.parseFloat(receivedString.substring(10,11));

            xyz[0] = map(xyz[0] ,  0, xy_max[0] ,  0.0, float(width));
            xyz[1] = map(xyz[1] ,  0, xy_max[1] ,  0.0, float(height));
        }
    }

    if (use_trail) {
        // show a trail behind the touch points:
        blendMode(SUBTRACT);
        fill(255, 6);
        rect(0, 0, width, height);
        blendMode(BLEND);
    } else {
        // clean the screen
        background(0);
    }

    // visualize if finger is away or not:
    if (xyz[2] > 0) {
        stroke(255);
        strokeWeight(0.05*height);
    } else {
        stroke(255, 0, 0); // RED!
        strokeWeight(0.15*height);
    }

    // invert x direction:
    point(width-xyz[0], xyz[1]);
}

