import processing.serial.*;

String receivedString = null;
Serial myPort;  // The serial port

float[] xy = {0,0};
float[] xy_max = {1260, 1043};


// TODO:


void setup() {
    size(1280, 720);
    noStroke();

    // List all the available serial ports
    printArray(Serial.list());
    myPort = new Serial(this, Serial.list()[0], 11500);
    myPort.clear();
    // Throw out the first reading, in case we started reading
    // in the middle of a string from the sender.
    receivedString = myPort.readStringUntil('\n');
    receivedString = null;
    println("Starting");
}

void draw() {
    while (myPort.available() > 0) {

        receivedString = myPort.readStringUntil('\n');
        if (receivedString != null) {

            if (receivedString.length() == 10) {

                if (receivedString.charAt(4) == '.') {
                    xy[0] = Float.parseFloat(receivedString.substring(0, 4));
                    xy[1] = Float.parseFloat(receivedString.substring(5, 9));
                }
            }
            println(xy[0], xy[1]);

            xy[0] = map(xy[0] ,  0, xy_max[0] ,  0.0, float(width));
            xy[1] = map(xy[1] ,  0, xy_max[1] ,  0.0, float(height));
            int radius = int(0.1 * height);

            background(0);
            ellipse(xy[0], height-xy[1], radius, radius);
        }
    }
}

