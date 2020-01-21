 
 
 
 
 glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
 
 
 
 glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+3);
   glTexCoord2f(texRepX,0.0); glVertex3f(+3,+1,-2.5);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0,+1,-2.5);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   
   
   cube(8.80,-0.1,yPos, 1,0.1,0.75, 90);
      cube(10.30,-0.1,yPos, 1,0.1,0.75, 90);
      cube(8.80,-0.1,yPos, 1,0.1,0.75, 90);
      cube(10.30,-0.1,yPos, 1,0.1,0.75, 90);
      
      for(i = 0; i < 7; i++){
   	  //ruas jalan -- jarak lajur : 1.5
      cube(xPos,-0.1,0.75, 1,0.1,0.75, 0);
      cube(xPos,-0.1,2.25, 1,0.1,0.75, 0);
      xPos += 2;
   }
   
   
   
   
   
   
   //Street surface - Main Street
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   texScale = 0.5;
   float xPos = -6;
 int i;
   for(i = 0; i < 7; i++){
   	  //ruas jalan -- jarak lajur : 1.5
      cube(xPos,-0.1,0.75, 1,0.1,0.75, 0);
      cube(xPos,-0.1,2.25, 1,0.1,0.75, 0);
      xPos += 2;
   }
   
   //turn-1-left
    turnCircuit(xPos,-0.1,0.75, 1,0.1,0.75, 0);
   
   
	//Street surface - turn left1
    glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   texScale = 0.5;
   float yPos = -2;
   float defTurnCoord = 0.7;
   for(i = 0; i < 4; i++){
      cube(xPos+defTurnCoord,-0.1,yPos, 1,0.1,0.75, 90);
      cube(xPos+defTurnCoord+1.5,-0.1,yPos, 1,0.1,0.75, 90);
      yPos -= 2;
   }
   	if(xPositionCar >= 6 && xPositionCar < 6.1 && zPositionCar < 3) {
     	turnMode(TURN_NORTHWEST, TURN_LEFT);  //turn-1
	} else if(zPositionCar >= -8.1 && zPositionCar <= -8 && xPositionCar > 8 && xPositionCar <= 9 ) {
		turnMode(TURN_NORTHWEST, TURN_RIGHT); //turn-2
	} else if(xPositionCar >= 15.5 && xPositionCar <= 15.6 && zPositionCar < -8) {
		turnMode(TURN_NORTHEAST, TURN_RIGHT); //turn-3
	} else if(zPositionCar >= 10.2 && zPositionCar <= 10.3 && xPositionCar > 15 ) {
		turnMode(TURN_SOUTHEAST, TURN_RIGHT); //turn-4
	} else if(xPositionCar >= -15.1 && xPositionCar <= -15 && zPositionCar > 12) {
		turnMode(TURN_SOUTHWEST, TURN_RIGHT); //turn-5
	} else if(zPositionCar >= -8.1 && zPositionCar <= -8 && xPositionCar < -16) {
		turnMode(TURN_NORTHWEST, TURN_RIGHT); //turn-6
	} else if(xPositionCar >= -12.1 && xPositionCar <= -12 && zPositionCar < -8) {
		turnMode(TURN_NORTHEAST, TURN_RIGHT); //turn-7
	} else if(zPositionCar >= -1.1 && zPositionCar <= -1 && xPositionCar < -9 && xPositionCar > -10) {
		turnMode(TURN_NORTHEAST, TURN_LEFT); //turn-8
	}
	
	//Car Movemen - Turn Counter
	if(turnDegreeCount >= turnCount) {
		if(isTurnNorthwest) {
			if(turnStatus == TURN_LEFT) {
				turnDegree = 90;
				isStright = true;	
			} else if (turnStatus == TURN_RIGHT) {
				turnDegree = 0;
				isStright = true;
			}
		}
		
		if(isTurnNortheast) {
			if(turnStatus == TURN_LEFT) {
				turnDegree = 0;
				isStright = true;	
			} else if (turnStatus == TURN_RIGHT) {
				turnDegree = 270;
				isStright = true;
			}
		}
		 
		if(isTurnSoutheast) {
			if(turnStatus == TURN_LEFT) {
				turnDegree = 270;
				isStright = true;	
			} else if (turnStatus == TURN_RIGHT) {
				turnDegree = 180;
				isStright = true;
			}
		}
		
		if(isTurnSouthwest) {
			if(turnStatus == TURN_LEFT) {
				turnDegree = 180;	
				isStright = true;	
			} else if (turnStatus == TURN_RIGHT) {
				turnDegree = 90;
				isStright = true;
			}
		}
	}
	
	//Car Movement : Speed
	if(isStright) {
		//Kalau dia jalan lurus
		if(turnDegree == 90) {
			zPositionCar = zPositionCar - speed;
		} else if(turnDegree == 270) {
			zPositionCar = zPositionCar + speed;
		} else if(turnDegree == 180) {
			xPositionCar = xPositionCar - speed;
		} else {
			xPositionCar = xPositionCar + speed;
		}
	}else {
		//Kalau dia belok
		if(isTurnNorthwest) {
			xPositionCar = xPositionCar + turnSpeed;
			zPositionCar = zPositionCar - turnSpeed;
			turnDegreeCount++;
		} else if(isTurnNortheast) {
			xPositionCar = xPositionCar + turnSpeed;
			zPositionCar = zPositionCar + turnSpeed;
			turnDegreeCount++;
		} else if(isTurnSoutheast) {
			xPositionCar = xPositionCar -turnSpeed;
			zPositionCar = zPositionCar + turnSpeed;
			turnDegreeCount++;
		} else if(isTurnSouthwest) {
			xPositionCar = xPositionCar - turnSpeed;
			zPositionCar = zPositionCar - turnSpeed;
			turnDegreeCount++;
		}
	}
   
   //Street surface - turn left1 duplicate in bottom (z coordinate)
   yPos =-8;
    for(i = 0; i < 4; i++){
      cube(-8.80,-0.1,yPos, 1,0.1,0.75, 90);
      cube(-10.30,-0.1,yPos, 1,0.1,0.75, 90);
      yPos += 2;
   }
   
   //turn-8-left
   turnCircuit(-8.80,-0.1,yPos, 1,0.1,0.75,270);
   
   //turn-2-right
    turnCircuit(xPos+2.90,-0.1,-10.7, 1,0.1,0.75,180);
   
    //Street surface - stright1
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   texScale = 0.5;
   float xPos2 = 13;
// int i;
   for(i = 0; i < 2; i++){
   	  //ruas jalan
      cube(xPos2-0.1,-0.1,-12.25, 1,0.1,0.75, 0);
      cube(xPos2-0.1,-0.1,-10.75, 1,0.1,0.75, 0);
      xPos2 += 2;
   }
   
	//Street surface - stright1 duplicate bottom
   xPos2 = -15;
   //turn-6-right
    turnCircuit(xPos2-2.20,-0.1,-10.7, 1,0.1,0.75,180);
   for(i = 0; i < 2; i++){
   	  //ruas jalan
      cube(xPos2-0.2,-0.1,-12.20, 1,0.1,0.75, 0);
      cube(xPos2-0.2,-0.1,-10.70, 1,0.1,0.75, 0);
      xPos2 += 2;
   }
   //turn-7right
   turnCircuit(xPos2+0.70,-0.1,-10, 1,0.1,0.75,90);
   
   		
   //turn-3right
   turnCircuit(xPos+9.75,-0.1,-10.10, 1,0.1,0.75,90);
   
   	//Street surface - turn right1
    glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   texScale = 0.5;
   float yPos2 = -8.25;
	
   for(i = 0; i < 10; i++){
      cube(17.80,-0.1,yPos2, 1,0.1,0.75, 90);
      cube(19.30,-0.1,yPos2, 1,0.1,0.75, 90);
      yPos2 += 2;
   }
   
   //turn-4-right
   turnCircuit(xPos+9.10,-0.1,12.60, 1,0.1,0.75,0);
   
     //Street surface - stright2
   glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   texScale = 0.5;
   float xPos3 = -15;
    //turn-5-left
   turnCircuit(xPos3-2.75,-0.1,12, 1,0.1,0.75,270);
   for(i = 0; i < 16; i++){
   	  //ruas jalan
      cube(xPos3+0.1,-0.1,12.65, 1,0.1,0.75, 0);
      cube(xPos3+0.1,-0.1,14.15, 1,0.1,0.75, 0);
      xPos3 += 2;
   }
   
  
   
   //Street surface - bottom
    glColor3f(0.4, 0.4, 0.4);
   glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
   texScale = 0.5;
   float yPos3 = -8;
	
   for(i = 0; i < 10; i++){
      cube(-17.80,-0.1,yPos3, 1,0.1,0.75, 90);
      cube(-19.30,-0.1,yPos3, 1,0.1,0.75, 90);
      yPos3 += 2;
   }
   
   
   
   
   
   
   
   //Dynamic move
   
   
   //Blue car
   car(xPositionCar[BLUE_CAR],0.13,zPositionCar[BLUE_CAR], 1,1,1, turnDegree[BLUE_CAR], 0,0,0.8);
	
   //Red Car
//   car(xPositionCar[RED_CAR],0.13,zPositionCar[RED_CAR], 1,1,1, turnDegree[RED_CAR], 0.8,0,0);
//   
//   //Black Car
//   car(xPositionCar[BLACK_CAR],0.13,zPositionCar[BLACK_CAR], 1,1,1, turnDegree[BLACK_CAR], 0,0,0);
//	printf("XPosition : %f, TurnDegree : %d, Zposition : %f\n ",
//	 xPositionCar, turnDegree, zPositionCar);
	
	//Car Movement
	float speed = 0.03;
	float turnSpeed = speed-0.01;
	int turnCount = (2.4/turnSpeed)+5;
	
	int car;
	for(car = 0; car < AMOUNT_OF_CARS; car++) {
//		printf("car : %d\n", car);
		if(xPositionCar[car] >= 6 && xPositionCar[car] < 6.1 && zPositionCar[car] < 3) {
	     	turnMode(TURN_NORTHWEST, TURN_LEFT, car);  //turn-1
		} else if(zPositionCar[car] >= -8.1 && zPositionCar[car] <= -8 && xPositionCar[car] > 8 && xPositionCar[car] <= 9 ) {
			turnMode(TURN_NORTHWEST, TURN_RIGHT, car); //turn-2
		} else if(xPositionCar[car] >= 15.5 && xPositionCar[car] <= 15.6 && zPositionCar[car] < -8) {
			turnMode(TURN_NORTHEAST, TURN_RIGHT, car); //turn-3
		} else if(zPositionCar[car] >= 10.2 && zPositionCar[car] <= 10.3 && xPositionCar[car] > 15 ) {
			turnMode(TURN_SOUTHEAST, TURN_RIGHT, car); //turn-4
		} else if(xPositionCar[car] >= -15.1 && xPositionCar[car] <= -15 && zPositionCar[car] > 12) {
			turnMode(TURN_SOUTHWEST, TURN_RIGHT, car); //turn-5
		} else if(zPositionCar[car] >= -8.1 && zPositionCar[car] <= -8 && xPositionCar[car] < -16) {
			turnMode(TURN_NORTHWEST, TURN_RIGHT, car); //turn-6
		} else if(xPositionCar[car] >= -12.1 && xPositionCar[car] <= -12 && zPositionCar[car] < -8) {
			turnMode(TURN_NORTHEAST, TURN_RIGHT, car); //turn-7
		} else if(zPositionCar[car] >= -1.1 && zPositionCar[car] <= -1 && xPositionCar[car] < -9 && xPositionCar[car] > -10) {
			turnMode(TURN_NORTHEAST, TURN_LEFT, car), car; //turn-8
		}
		
		//Car Movemen - Turn Counter
		if(turnDegreeCount >= turnCount) {
			if(isTurnNorthwest) {
				if(turnStatus == TURN_LEFT) {
					turnDegree[car] = 90;
					isStright = true;	
				} else if (turnStatus == TURN_RIGHT) {
					turnDegree[car] = 0;
					isStright = true;
				}
			}
			
			if(isTurnNortheast) {
				if(turnStatus == TURN_LEFT) {
					turnDegree[car]= 0;
					isStright = true;	
				} else if (turnStatus == TURN_RIGHT) {
					turnDegree[car] = 270;
					isStright = true;
				}
			}
			 
			if(isTurnSoutheast) {
				if(turnStatus == TURN_LEFT) {
					turnDegree[car] = 270;
					isStright = true;	
				} else if (turnStatus == TURN_RIGHT) {
					turnDegree[car] = 180;
					isStright = true;
				}
			}
			
			if(isTurnSouthwest) {
				if(turnStatus == TURN_LEFT) {
					turnDegree[car] = 180;	
					isStright = true;	
				} else if (turnStatus == TURN_RIGHT) {
					turnDegree[car] = 90;
					isStright = true;
				}
			}
		}
		
		//Car Movement : Speed
		if(isStright) {
			//Kalau dia jalan lurus
			if(turnDegree[car] == 90) {
				zPositionCar[car] = zPositionCar[car] - speed;
			} else if(turnDegree[car] == 270) {
				zPositionCar[car] = zPositionCar[car] + speed;
			} else if(turnDegree[car] == 180) {
				xPositionCar[car] = xPositionCar[car] - speed;
			} else {
				xPositionCar[car] = xPositionCar[car] + speed;
			}
		}else {
			//Kalau dia belok
			printf("Masuk ke-%d\n", turnDegreeCount);
			if(isTurnNorthwest) {
				xPositionCar[car] = xPositionCar[car] + turnSpeed;
				zPositionCar[car] = zPositionCar[car] - turnSpeed;
				turnDegreeCount++;
			} else if(isTurnNortheast) {
				xPositionCar[car] = xPositionCar[car] + turnSpeed;
				zPositionCar[car] = zPositionCar[car] + turnSpeed;
				turnDegreeCount++;
			} else if(isTurnSoutheast) {
				xPositionCar[car] = xPositionCar[car] - turnSpeed;
				zPositionCar[car] = zPositionCar[car] + turnSpeed;
				turnDegreeCount++;
			} else if(isTurnSouthwest) {
				xPositionCar[car] = xPositionCar[car] - turnSpeed;
				zPositionCar[car] = zPositionCar[car] - turnSpeed;
				turnDegreeCount++;
			}
		}
	}
	
	
   //Static
     //Blue car
   car(xPositionCar,0.13,zPositionCar, 1,1,1, turnDegree, 0,0,0.8);

	//Car Movement
	float speed = 0.03;
	float turnSpeed = speed-0.01;
	int turnCount = (2.4/turnSpeed)+5;
	
   	if(xPositionCar >= 6 && xPositionCar < 6.1 && zPositionCar < 3) {
     	turnMode(TURN_NORTHWEST, TURN_LEFT);  //turn-1
	} else if(zPositionCar >= -8.1 && zPositionCar <= -8 && xPositionCar > 8 && xPositionCar <= 9 ) {
		turnMode(TURN_NORTHWEST, TURN_RIGHT); //turn-2
	} else if(xPositionCar >= 15.5 && xPositionCar <= 15.6 && zPositionCar < -8) {
		turnMode(TURN_NORTHEAST, TURN_RIGHT); //turn-3
	} else if(zPositionCar >= 10.2 && zPositionCar <= 10.3 && xPositionCar > 15 ) {
		turnMode(TURN_SOUTHEAST, TURN_RIGHT); //turn-4
	} else if(xPositionCar >= -15.1 && xPositionCar <= -15 && zPositionCar > 12) {
		turnMode(TURN_SOUTHWEST, TURN_RIGHT); //turn-5
	} else if(zPositionCar >= -8.1 && zPositionCar <= -8 && xPositionCar < -16) {
		turnMode(TURN_NORTHWEST, TURN_RIGHT); //turn-6
	} else if(xPositionCar >= -12.1 && xPositionCar <= -12 && zPositionCar < -8) {
		turnMode(TURN_NORTHEAST, TURN_RIGHT); //turn-7
	} else if(zPositionCar >= -1.1 && zPositionCar <= -1 && xPositionCar < -9 && xPositionCar > -10) {
		turnMode(TURN_NORTHEAST, TURN_LEFT); //turn-8
	}
	
	//Car Movemen - Turn Counter
	if(turnDegreeCount >= turnCount) {
		if(isTurnNorthwest) {
			if(turnStatus == TURN_LEFT) {
				turnDegree = 90;
				isStright = true;	
			} else if (turnStatus == TURN_RIGHT) {
				turnDegree = 0;
				isStright = true;
			}
		}
		
		if(isTurnNortheast) {
			if(turnStatus == TURN_LEFT) {
				turnDegree = 0;
				isStright = true;	
			} else if (turnStatus == TURN_RIGHT) {
				turnDegree = 270;
				isStright = true;
			}
		}
		 
		if(isTurnSoutheast) {
			if(turnStatus == TURN_LEFT) {
				turnDegree = 270;
				isStright = true;	
			} else if (turnStatus == TURN_RIGHT) {
				turnDegree = 180;
				isStright = true;
			}
		}
		
		if(isTurnSouthwest) {
			if(turnStatus == TURN_LEFT) {
				turnDegree = 180;	
				isStright = true;	
			} else if (turnStatus == TURN_RIGHT) {
				turnDegree = 90;
				isStright = true;
			}
		}
	}
	
	//Car Movement : Speed
	if(isStright) {
		//Kalau dia jalan lurus
		if(turnDegree == 90) {
			zPositionCar = zPositionCar - speed;
		} else if(turnDegree == 270) {
			zPositionCar = zPositionCar + speed;
		} else if(turnDegree == 180) {
			xPositionCar = xPositionCar - speed;
		} else {
			xPositionCar = xPositionCar + speed;
		}
	}else {
		//Kalau dia belok
//			printf("Masuk ke-%d\n", turnDegreeCount);
		if(isTurnNorthwest) {
			printf("Masuk ke-%d\n", turnDegreeCount);
			xPositionCar = xPositionCar + turnSpeed;
			zPositionCar = zPositionCar - turnSpeed;
			turnDegreeCount++;
		} else if(isTurnNortheast) {
			xPositionCar = xPositionCar + turnSpeed;
			zPositionCar = zPositionCar + turnSpeed;
			turnDegreeCount++;
		} else if(isTurnSoutheast) {
			xPositionCar = xPositionCar -turnSpeed;
			zPositionCar = zPositionCar + turnSpeed;
			turnDegreeCount++;
		} else if(isTurnSouthwest) {
			xPositionCar = xPositionCar - turnSpeed;
			zPositionCar = zPositionCar - turnSpeed;
			turnDegreeCount++;
		}
	}
