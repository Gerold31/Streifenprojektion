SYSTEM_MODE(SEMI_AUTOMATIC);

Servo servo;

int laser = D0;

void setup()
{
	WiFi.off();

	RGB.control(true);
	RGB.color(255, 0, 0);

	Serial.begin(19200);

	servo.attach(A0);

	pinMode(laser, OUTPUT);
	digitalWrite(laser, HIGH);
}

void loop()
{
	if(Serial.available() >= 2)
	{
		char cmd = Serial.read();
		char val = Serial.read();
		switch(cmd)
		{
			case 'm':
				servo.write(val);
				break;
			case 'l':
				if(val == '0')
					digitalWrite(laser, HIGH);
				else
					digitalWrite(laser, LOW);
				break;
		}
	}
}
