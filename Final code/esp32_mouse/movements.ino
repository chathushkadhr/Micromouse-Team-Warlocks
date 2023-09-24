void stop_motors()
{
  ledcWrite(PWMm1a, 0);
  ledcWrite(PWMm1b, 0);
  ledcWrite(PWMm2a, 0);
  ledcWrite(PWMm2b, 0);
  delay(200);
}

void turn_align(int dir)
{
  //    adjustCell();
  mpu.update();
  float f = readTOF(frontTOF);
  if (f > wall_thresh)
  {
    align();
    turn90(dir);
  }
  else
  {
    correct_direction();
    turn90(dir);
    align();
  }
  mpu.update();
  globle_theta = mpu.getAngleZ();
}

void turn90(int dir)
{
  // dir=1 for left turn
  // dir=-1 for right turn
  // using encoders
  int right_dir, left_dir;

  if (dir == 1)
  {
    right_dir = 1, left_dir = -1;
  }
  else
  {
    right_dir = -1, left_dir = 1;
  }

  int turn_speed1 = 0, turn_speed2 = 0;
  float pre_error1 = 0, pre_error2 = 0, dt = 0, tot_error1 = 0, tot_error2 = 0;
  double f_t;
  count1 = 0;
  count2 = 0;
  float error1 = right_dir * turnCount, error2 = left_dir * turnCount;

  mpu.update();
  float initial_yaw = mpu.getAngleZ();
  float desired_yaw;
  if (dir == 1)
  {
    desired_yaw = initial_yaw - 90;
  }
  else
  {
    desired_yaw = initial_yaw + 90;
  }

  while (abs(error1) > 0.1 || abs(error2) > 0.1)
  {
    mpu.update();
    float turn_speed_f1 = kp_turn * abs(error1) + kd_turn * (error1 - pre_error1);
    float turn_speed_f2 = kp_turn * abs(error2) + kd_turn * (error2 - pre_error2);
    if (turn_speed_f1 < minpwm)
    {
      turn_speed1 = minpwm;
    }
    else if (turn_speed_f1 > maxpwm)
    {
      turn_speed1 = maxpwm;
    }
    else
    {
      turn_speed1 = (int)turn_speed_f1;
    }
    if (turn_speed_f2 < minpwm)
    {
      turn_speed2 = minpwm;
    }
    else if (turn_speed_f2 > maxpwm)
    {
      turn_speed2 = maxpwm;
    }
    else
    {
      turn_speed2 = (int)turn_speed_f2;
    }

    if (error1 > 0)
    {
      // forward rotation
      ledcWrite(PWMm1a, 0);
      ledcWrite(PWMm1b, turn_speed1);
    }
    else
    {
      // backward rotation
      ledcWrite(PWMm1a, turn_speed1);
      ledcWrite(PWMm1b, 0);
    }

    if (error2 > 0)
    {
      // forward rotation
      ledcWrite(PWMm2a, 0);
      ledcWrite(PWMm2b, turn_speed2);
    }
    else
    {
      // backward rotation
      ledcWrite(PWMm2a, turn_speed2);
      ledcWrite(PWMm2b, 0);
    }
    delay(5);
    pre_error1 = error1;
    error1 = (right_dir * turnCount - count1);

    pre_error2 = error2;
    error2 = (left_dir * turnCount - count2);
  }

  ledcWrite(PWMm1a, 0);
  ledcWrite(PWMm1b, 0);
  ledcWrite(PWMm2a, 0);
  ledcWrite(PWMm2b, 0);
  delay(50);

  mpu.update();
  if (abs(mpu.getAngleZ() - desired_yaw) > 5)
  {
    //      SerialBT.println(mpu.getAngleZ() - desired_yaw);
    mpu.update();
    while (abs(mpu.getAngleZ() - desired_yaw) > 0.1)
    {
      float pre_yaw_error = 0;
      while (true)
      {
        mpu.update();
        float yaw_error = desired_yaw - mpu.getAngleZ();

        if (abs(yaw_error) < 0.1)
        {
          ledcWrite(PWMm1a, 0);
          ledcWrite(PWMm1b, 0);
          ledcWrite(PWMm2a, 0);
          ledcWrite(PWMm2b, 0);
          break;
        }

        float speedR = (kp_gyro_turn * abs(yaw_error) + kd_gyro_turn * (yaw_error - pre_yaw_error));
        float speedL = (kp_gyro_turn * abs(yaw_error) + kd_gyro_turn * (yaw_error - pre_yaw_error));

        if (speedR < minpwm)
        {
          speedR = minpwm;
        }
        else if (speedR > maxpwm)
        {
          speedR = maxpwm;
        }
        else
        {
          speedR = (int)speedR;
        }
        if (speedL < minpwm)
        {
          speedL = minpwm;
        }
        else if (speedL > maxpwm)
        {
          speedL = maxpwm;
        }
        else
        {
          speedL = (int)speedL;
        }

        if (yaw_error > 0)
        {
          ledcWrite(PWMm1a, abs(speedL));
          ledcWrite(PWMm1b, 0);
          ledcWrite(PWMm2a, 0);
          ledcWrite(PWMm2b, abs(speedR));
          delay(10);
        }
        else
        {
          ledcWrite(PWMm1a, 0);
          ledcWrite(PWMm1b, abs(speedL));
          ledcWrite(PWMm2a, abs(speedR));
          ledcWrite(PWMm2b, 0);
          delay(10);
        }
        pre_yaw_error = yaw_error;
      }
      mpu.update();
    }
  }
}

void align()
{

  float error = 0;
  int d = 0;

  float l1 = readTOF(leftTOF1);
  delay(2);
  float l2 = readTOF(leftTOF2);
  delay(2);

  if (l1 > wall_thresh || l2 > wall_thresh)
  {
    float r1 = readTOF(rightTOF1);
    delay(2);
    float r2 = readTOF(rightTOF2);
    delay(2);
    if (r1 > wall_thresh || r2 > wall_thresh)
    {
      ledcWrite(PWMm1a, 0);
      ledcWrite(PWMm1b, 0);
      ledcWrite(PWMm2a, 0);
      ledcWrite(PWMm2b, 0);
      delay(10);
      return;
    }
    else
    {
      d = 0;
      error = r2 - r1;
    }
  }
  else
  {
    d = 1;
    error = l1 - l2;
  }

  float prevError = 0;
  float totalError = 0;

  unsigned long time_start = millis();

  while ((abs(error) > 3))
  {
    while (abs(error) > 2)
    {
      if (millis() - time_start > ALIGN_TIME)
      {
        ledcWrite(PWMm1a, 0);
        ledcWrite(PWMm1b, 0);
        ledcWrite(PWMm2a, 0);
        ledcWrite(PWMm2b, 0);
        delay(10);
        return;
      }
      mpu.update();
      int velocity = error * kp_align + (error - prevError) * kd_align;

      if (velocity < -1 * maxpwm)
      {
        velocity = -1 * maxpwm;
      }
      else if (velocity > maxpwm)
      {
        velocity = maxpwm;
      }
      else
      {
        velocity = (int)velocity;
      }
      if (velocity != 0)
      {
        if (abs(velocity) < 210)
        {
          velocity = 210 * velocity / abs(velocity);
          velocity = (int)velocity;
        }
      }

      if (velocity < 0)
      {
        ledcWrite(PWMm1a, abs(velocity));
        ledcWrite(PWMm1b, 0);
        ledcWrite(PWMm2a, 0);
        ledcWrite(PWMm2b, abs(velocity));
        delay(2);
      }
      else
      {
        ledcWrite(PWMm1a, 0);
        ledcWrite(PWMm1b, abs(velocity));
        ledcWrite(PWMm2a, abs(velocity));
        ledcWrite(PWMm2b, 0);
        delay(2);
      }

      prevError = error;
      if (d == 1)
      {
        error = readTOF(leftTOF1) - readTOF(leftTOF2);
      }
      else
      {
        error = readTOF(rightTOF2) - readTOF(rightTOF1);
      }
    }

    ledcWrite(PWMm1a, 0);
    ledcWrite(PWMm1b, 0);
    ledcWrite(PWMm2a, 0);
    ledcWrite(PWMm2b, 0);
    delay(5);

    if (d == 1)
    {
      float l1 = readTOF(leftTOF1);
      delay(2);
      float l2 = readTOF(leftTOF2);
      delay(2);
      error = l1 - l2;
    }
    else
    {
      float r1 = readTOF(rightTOF1);
      delay(2);
      float r2 = readTOF(rightTOF2);
      delay(2);
      error = r2 - r1;
    }
  }

  mpu.update();
  globle_theta = mpu.getAngleZ();
}

void goCellDevel()
{

  count1 = 0;
  count2 = 0;
  mpu.update();

  while ((count1 + count2) / 2 < cellCount)
  {
    float tof_error = 0;

    if (prev_l)
    {
      float l1 = readTOF(leftTOF1);
      delay(2);
      if (l1 < wall_thresh)
      {
        tof_error = (l1 - left_distance) / 10;
      }
      else
      {
        float r1 = readTOF(rightTOF1);
        delay(2);
        if (r1 < wall_thresh)
        {
          tof_error = (right_distance - r1) / 10;
          prev_l = false;
          prev_r = true;
        }
      }
    }

    else if (prev_r)
    {
      float r1 = readTOF(rightTOF1);
      delay(2);
      if (r1 < wall_thresh)
      {
        tof_error = (right_distance - r1) / 10;
      }
      else
      {
        float l1 = readTOF(leftTOF1);
        delay(2);
        if (l1 < wall_thresh)
        {
          tof_error = (l1 - left_distance) / 10;
          prev_r = false;
          prev_l = true;
        }
      }
    }

    if (abs(tof_error) < 1)
    {
      tof_error = 0;
    }

    mpu.update();
    float error = (mpu.getAngleZ() - globle_theta) + tof_error * k_tof;

    float speedR = midpwm + r - (kp_gyro * error + kd_gyro * (error - pre_error));
    float speedL = midpwm + (kp_gyro * error + kd_gyro * (error - pre_error));

    if (speedR < minpwm)
    {
      speedR = minpwm + r;
    }
    else if (speedR > maxpwm + r)
    {
      speedR = maxpwm + r;
    }
    else
    {
      speedR = (int)speedR;
    }
    if (speedL < minpwm)
    {
      speedL = minpwm;
    }
    else if (speedL > maxpwm)
    {
      speedL = maxpwm;
    }
    else
    {
      speedL = (int)speedL;
    }

    ledcWrite(PWMm1a, 0);
    ledcWrite(PWMm1b, speedL);
    ledcWrite(PWMm2a, 0);
    ledcWrite(PWMm2b, speedR);

    delay(2);
    pre_error = error;

    float f = (readTOF(frontTOF));

    if (f < 120)
    {
      float speed_front = 0;

      while (true)
      {
        float error_front = 0;
        float pre_error_front = 0;
        while (true)
        {
          if (abs(f - 38) < 2)
          {
            break;
          }

          error_front = (f - 38);

          speed_front = (kp_front_tof * error_front + kd_front_tof * (error_front - pre_error_front));

          if (speed_front < -1 * maxpwm)
          {
            speed_front = -1 * maxpwm;
          }
          if (speed_front > maxpwm)
          {
            speed_front = maxpwm;
          }

          if (abs(speed_front) < minpwm)
          {
            speed_front = minpwm * (speed_front / abs(speed_front));
          }

          speed_front = (int)speed_front;

          if (speed_front > 0)
          {
            ledcWrite(PWMm1a, 0);
            ledcWrite(PWMm1b, speed_front);
            ledcWrite(PWMm2a, 0);
            ledcWrite(PWMm2b, speed_front);
          }
          else
          {
            ledcWrite(PWMm1a, -1 * speed_front);
            ledcWrite(PWMm1b, 0);
            ledcWrite(PWMm2a, -1 * speed_front);
            ledcWrite(PWMm2b, 0);
          }
          delay(2);

          pre_error_front = error_front;
          f = (readTOF(frontTOF));
        }
        f = (readTOF(frontTOF));
        if (abs(f - 38) < 2)
        {
          break;
        }
      }

      ledcWrite(PWMm1a, 0);
      ledcWrite(PWMm1b, 0);
      ledcWrite(PWMm2a, 0);
      ledcWrite(PWMm2b, 0);
      delay(10);
      Buzz(cellTone);
      need_break = 0;
      return;
    }
  }
  Buzz(cellTone);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,210);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,210);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,0);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,0);
  //    delay(10);
  breakNow2(breakCount);
  need_break = 1;
}

void goCellDevel2()
{
  //
  //  float kp_gyro =40, kd_gyro =80, k_tof =4;
  //  int cellCount =260;

  count1 = 0;
  count2 = 0;
  mpu.update();
  //
  float IR_prev_l = readIR2(leftIR);
  float IR_prev_r = readIR2(rightIR);

  while ((count1 + count2) / 2 < cellCount)
  {
    float tof_error = 0;

    if (prev_l)
    {
      float l1 = readTOF(leftTOF1);
      delay(2);
      if (l1 < wall_thresh)
      {
        tof_error = (l1 - left_distance) / 10;
      }
      else
      {
        float r1 = readTOF(rightTOF1);
        delay(2);
        if (r1 < wall_thresh)
        {
          tof_error = (right_distance - r1) / 10;
          prev_l = false;
          prev_r = true;
        }
      }
    }

    else if (prev_r)
    {
      float r1 = readTOF(rightTOF1);
      delay(2);
      if (r1 < wall_thresh)
      {
        tof_error = (right_distance - r1) / 10;
      }
      else
      {
        float l1 = readTOF(leftTOF1);
        delay(2);
        if (l1 < wall_thresh)
        {
          tof_error = (l1 - left_distance) / 10;
          prev_r = false;
          prev_l = true;
        }
      }
    }

    if (abs(tof_error) < 1)
    {
      tof_error = 0;
    }

    mpu.update();
    float error = (mpu.getAngleZ() - globle_theta) + tof_error * k_tof;

    float speedR = midpwm + r - (kp_gyro * error + kd_gyro * (error - pre_error));
    float speedL = midpwm + (kp_gyro * error + kd_gyro * (error - pre_error));

    if (speedR < minpwm)
    {
      speedR = minpwm + r;
    }
    else if (speedR > maxpwm + r)
    {
      speedR = maxpwm + r;
    }
    else
    {
      speedR = (int)speedR;
    }
    if (speedL < minpwm)
    {
      speedL = minpwm;
    }
    else if (speedL > maxpwm)
    {
      speedL = maxpwm;
    }
    else
    {
      speedL = (int)speedL;
    }

    ledcWrite(PWMm1a, 0);
    ledcWrite(PWMm1b, speedL);
    ledcWrite(PWMm2a, 0);
    ledcWrite(PWMm2b, speedR);

    delay(2);
    pre_error = error;

    float f = (readTOF(frontTOF));

    if (f < 120)
    {
      float speed_front = 0;

      while (true)
      {
        float error_front = 0;
        float pre_error_front = 0;
        while (true)
        {
          if (abs(f - 38) < 2)
          {
            break;
          }

          error_front = (f - 38);

          speed_front = (kp_front_tof * error_front + kd_front_tof * (error_front - pre_error_front));

          if (speed_front < -1 * maxpwm)
          {
            speed_front = -1 * maxpwm;
          }
          if (speed_front > maxpwm)
          {
            speed_front = maxpwm;
          }

          if (abs(speed_front) < minpwm)
          {
            speed_front = minpwm * (speed_front / abs(speed_front));
          }

          speed_front = (int)speed_front;

          if (speed_front > 0)
          {
            ledcWrite(PWMm1a, 0);
            ledcWrite(PWMm1b, speed_front);
            ledcWrite(PWMm2a, 0);
            ledcWrite(PWMm2b, speed_front);
          }
          else
          {
            ledcWrite(PWMm1a, -1 * speed_front);
            ledcWrite(PWMm1b, 0);
            ledcWrite(PWMm2a, -1 * speed_front);
            ledcWrite(PWMm2b, 0);
          }
          delay(2);

          pre_error_front = error_front;
          f = (readTOF(frontTOF));
        }
        f = (readTOF(frontTOF));
        if (abs(f - 38) < 2)
        {
          break;
        }
      }

      ledcWrite(PWMm1a, 0);
      ledcWrite(PWMm1b, 0);
      ledcWrite(PWMm2a, 0);
      ledcWrite(PWMm2b, 0);
      delay(10);
      Buzz(cellTone);
      return;
    }

    float IR_left = readIR(leftIR);
    float IR_right = readIR(rightIR);

    if (((IR_prev_l < IR_THRESH) && (IR_left > IR_THRESH)) || ((IR_prev_r < IR_THRESH) && (IR_right > IR_THRESH)))
    {
      need_break = 0;
      breakNow2(80);
      delay(20);
      Buzz(cellTone);
      return;
    }
  }
  Buzz(cellTone);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,210);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,210);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,0);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,0);
  //    delay(10);
  breakNow2(breakCount);
}

void breakNow2(int bC)
{
  count1 = 0;
  count2 = 0;
  int break_count_ = bC;

  float error = break_count_ - ((count1 + count2)) / 2;
  float pre_error = 0;
  unsigned long time_start = millis();
  while (true)
  {
    mpu.update();
    if (abs(error) < 3)
    {
      break;
    }

    while (true)
    {
      mpu.update();
      if (millis() - time_start > 3000)
      {
        ledcWrite(PWMm1a, 0);
        ledcWrite(PWMm1b, 0);
        ledcWrite(PWMm2a, 0);
        ledcWrite(PWMm2b, 0);
        delay(10);
        return;
      }
      if (abs(error) < 3)
      {
        ledcWrite(PWMm1a, 0);
        ledcWrite(PWMm1b, 0);
        ledcWrite(PWMm2a, 0);
        ledcWrite(PWMm2b, 0);
        delay(10);
        break;
      }

      float motorSpeed = kp_cell * error + kd_cell * (error - pre_error);

      if (motorSpeed < -1 * maxpwm)
      {
        motorSpeed = -1 * maxpwm;
      }
      else if (motorSpeed > maxpwm)
      {
        motorSpeed = maxpwm;
      }
      else
      {
        motorSpeed = (int)motorSpeed;
      }
      if (motorSpeed != 0)
      {
        if (abs(motorSpeed) < minpwm)
        {
          motorSpeed = minpwm * motorSpeed / abs(motorSpeed);
          motorSpeed = (int)motorSpeed;
        }
      }

      if (motorSpeed > 0)
      {
        ledcWrite(PWMm1a, 0);
        ledcWrite(PWMm1b, abs(motorSpeed));
        ledcWrite(PWMm2a, 0);
        ledcWrite(PWMm2b, abs(motorSpeed));
        delay(2);
      }
      else
      {
        ledcWrite(PWMm1a, motorSpeed);
        ledcWrite(PWMm1b, 0);
        ledcWrite(PWMm2a, motorSpeed);
        ledcWrite(PWMm2b, 0);
        delay(2);
      }
      pre_error = error;
      error = break_count_ - ((count1 + count2)) / 2;
    }
    error = break_count_ - ((count1 + count2)) / 2;
  }
  //  SerialBT.println("Cell count;");
  //  SerialBT.println((count1 + count2)/2);
}

void go_count(int total_count)
{
  count1 = 0;
  count2 = 0;

  float error = total_count - ((count1 + count2)) / 2;
  float pre_error = 0;

  while (true)
  {
    if (abs(error) < 2)
    {
      break;
    }

    while (true)
    {
      if (abs(error) < 2)
      {
        ledcWrite(PWMm1a, 0);
        ledcWrite(PWMm1b, 0);
        ledcWrite(PWMm2a, 0);
        ledcWrite(PWMm2b, 0);
        delay(10);
        break;
      }

      float motorSpeed = kp_cell * error + kd_cell * (error - pre_error);

      if (motorSpeed < -1 * maxpwm)
      {
        motorSpeed = -1 * maxpwm;
      }
      else if (motorSpeed > maxpwm)
      {
        motorSpeed = maxpwm;
      }
      else
      {
        motorSpeed = (int)motorSpeed;
      }
      if (motorSpeed != 0)
      {
        if (abs(motorSpeed) < minpwm)
        {
          motorSpeed = minpwm * motorSpeed / abs(motorSpeed);
          motorSpeed = (int)motorSpeed;
        }
      }

      if (motorSpeed > 0)
      {
        ledcWrite(PWMm1a, 0);
        ledcWrite(PWMm1b, abs(motorSpeed));
        ledcWrite(PWMm2a, 0);
        ledcWrite(PWMm2b, abs(motorSpeed));
        delay(2);
      }
      else
      {
        ledcWrite(PWMm1a, abs(motorSpeed));
        ledcWrite(PWMm1b, 0);
        ledcWrite(PWMm2a, abs(motorSpeed));
        ledcWrite(PWMm2b, 0);
        delay(2);
      }
      pre_error = error;
      error = total_count - ((count1 + count2)) / 2;
    }
    error = total_count - ((count1 + count2)) / 2;
  }
  //  SerialBT.println("Cell count;");
  //  SerialBT.println((count1 + count2)/2);
}

void adjustCell()
{
  //
  float l1 = readTOF(leftTOF1);
  delay(5);
  float l2 = readTOF(leftTOF2);
  delay(5);
  float l_ir = readIR2(leftIR);
  delay(2);
  //  SerialBT.println(l1);
  //  SerialBT.println(" ");
  //  SerialBT.println(l2);
  //  SerialBT.println(" ");
  //  SerialBT.println(l_ir);
  //  SerialBT.println(" ");

  if (((l1 < wall_thresh) && (l_ir < IR_THRESH) && (l2 > wall_thresh)) || ((l1 > wall_thresh) && (l_ir > IR_THRESH) && (l2 < wall_thresh)))
  {
    go_count(100);
    return;
  }

  else if (((l2 < wall_thresh) && (l_ir < IR_THRESH) && (l1 > wall_thresh)) || ((l2 > wall_thresh) && (l_ir > IR_THRESH) && (l1 < wall_thresh)))
  {
    go_count(-100);
    return;
  }

  float r1 = readTOF(rightTOF1);
  delay(5);
  float r2 = readTOF(rightTOF2);
  delay(5);
  float r_ir = readIR2(rightIR);
  delay(2);

  if (((r1 < wall_thresh) && (r_ir < IR_THRESH) && (r2 > wall_thresh)) || ((r1 > wall_thresh) && (r_ir > IR_THRESH) && (r2 < wall_thresh)))
  {

    go_count(100);
    return;
  }

  else if (((r2 < wall_thresh) && (r_ir < IR_THRESH) && (r1 > wall_thresh)) || ((r2 > wall_thresh) && (r_ir > IR_THRESH) && (r1 < wall_thresh)))
  {
    go_count(-100);
    return;
  }
}

void goCellDevelIR()
{
  int maxpwm = 500;
  int midpwm = 370;
  count1 = 0;
  count2 = 0;
  mpu.update();

  float IR_prev_l = readIR2(leftIR);
  float IR_prev_r = readIR2(rightIR);

  while ((count1 + count2) / 2 < cellCount)
  {
    float ir_error = 0;

    float l = readIR(leftIR);
    float r = readIR(rightIR);

    if (((IR_prev_l < IR_THRESH) && (l > IR_THRESH)) || ((IR_prev_r < IR_THRESH) && (r > IR_THRESH)))
    {
      need_break = 0;
      breakNow2(120);
      delay(2);
      Buzz(cellTone);
      return;
    }

    if ((l < IR_THRESH) && (r < IR_THRESH))
    {
      if (l > r)
      {
        ir_error = l - ir_left_distance;
      }
      else
      {
        ir_error = ir_right_distance - r;
      }
    }

    else
    {
      if (l < IR_THRESH)
      {
        ir_error = l - ir_left_distance;
      }
      else if (r < IR_THRESH)
      {
        ir_error = ir_right_distance - r;
      }
    }
//    if (abs(ir_error) < 1)
//    {
//      ir_error = 0;
//    }

    mpu.update();
    float error = (mpu.getAngleZ() - globle_theta) + ir_error * k_ir;

    float speedR = midpwm - (kp_gyro * error + kd_gyro * (error - pre_error));
    float speedL = midpwm + (kp_gyro * error + kd_gyro * (error - pre_error));

    if (speedR < minpwm)
    {
      speedR = minpwm;
    }
    else if (speedR > maxpwm)
    {
      speedR = maxpwm;
    }
    else
    {
      speedR = (int)speedR;
    }
    if (speedL < minpwm)
    {
      speedL = minpwm;
    }
    else if (speedL > maxpwm)
    {
      speedL = maxpwm;
    }
    else
    {
      speedL = (int)speedL;
    }

    ledcWrite(PWMm1a, 0);
    ledcWrite(PWMm1b, speedL);
    ledcWrite(PWMm2a, 0);
    ledcWrite(PWMm2b, speedR);

    delay(2);
    pre_error = error;

    float f = (readTOF(frontTOF));

//    unsigned long time_start = 0;

    if (f < 140)
    {
      float speed_front = 0;
      unsigned long time_start = millis();
      while (true)
      {
        mpu.update();
        float error_front = 0;
        float pre_error_front = 0;
        while (true)
        {
          if ((millis() - time_start) > ALIGN_TIME && time_start != 0)
          {
            ledcWrite(PWMm1a, 0);
            ledcWrite(PWMm1b, 0);
            ledcWrite(PWMm2a, 0);
            ledcWrite(PWMm2b, 0);
            delay(10);
            Buzz(cellTone);
            need_break = 0;
            return;
          }
          if (abs(f - 38) < 2)
          {
            if (time_start == 0)
              time_start = millis();
            break;
          }

          error_front = (f - 38);

          speed_front = (kp_front_tof * error_front + kd_front_tof * (error_front - pre_error_front));

          if (speed_front < -1 * maxpwm)
          {
            speed_front = -1 * maxpwm;
          }
          if (speed_front > maxpwm)
          {
            speed_front = maxpwm;
          }

          if (abs(speed_front) < minpwm)
          {
            speed_front = minpwm * (speed_front / abs(speed_front));
          }

          speed_front = (int)speed_front;

          if (speed_front > 0)
          {
            ledcWrite(PWMm1a, 0);
            ledcWrite(PWMm1b, speed_front);
            ledcWrite(PWMm2a, 0);
            ledcWrite(PWMm2b, speed_front);
          }
          else
          {
            ledcWrite(PWMm1a, -1 * speed_front);
            ledcWrite(PWMm1b, 0);
            ledcWrite(PWMm2a, -1 * speed_front);
            ledcWrite(PWMm2b, 0);
          }
          delay(2);

          pre_error_front = error_front;
          f = (readTOF(frontTOF));
        }

        f = (readTOF(frontTOF));
        if (abs(f - 38) < 2)
        {
          break;
        }
      }

      ledcWrite(PWMm1a, 0);
      ledcWrite(PWMm1b, 0);
      ledcWrite(PWMm2a, 0);
      ledcWrite(PWMm2b, 0);
      delay(10);
      Buzz(cellTone);
      need_break = 0;
      return;
    }
  }
  Buzz(cellTone);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,210);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,210);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,0);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,0);
  //    delay(10);
  breakNow2(breakCount);
  need_break = 1;
}

void correct_direction()
{

  mpu.update();
  if (abs(mpu.getAngleZ() - globle_theta) > 4)
  {
    mpu.update();
    float pre_yaw_error = 0;
    unsigned long time_start = millis();

    while (abs(mpu.getAngleZ() - globle_theta) > 2)
    {
      while (true)
      {
        if (millis() - time_start > ALIGN_TIME)
        {
          ledcWrite(PWMm1a, 0);
          ledcWrite(PWMm1b, 0);
          ledcWrite(PWMm2a, 0);
          ledcWrite(PWMm2b, 0);
          delay(10);
          return;
        }

        mpu.update();
        float yaw_error = globle_theta - mpu.getAngleZ();

        if (abs(yaw_error) < 2)
        {
          ledcWrite(PWMm1a, 0);
          ledcWrite(PWMm1b, 0);
          ledcWrite(PWMm2a, 0);
          ledcWrite(PWMm2b, 0);
          delay(10);
          break;
        }

        float speedR = (kp_gyro_turn * abs(yaw_error) + kd_gyro_turn * (yaw_error - pre_yaw_error));
        float speedL = (kp_gyro_turn * abs(yaw_error) + kd_gyro_turn * (yaw_error - pre_yaw_error));

        if (speedR < minpwm)
        {
          speedR = minpwm;
        }
        else if (speedR > maxpwm)
        {
          speedR = maxpwm;
        }
        else
        {
          speedR = (int)speedR;
        }
        if (speedL < minpwm)
        {
          speedL = minpwm;
        }
        else if (speedL > maxpwm)
        {
          speedL = maxpwm;
        }
        else
        {
          speedL = (int)speedL;
        }

        if (yaw_error > 0)
        {
          ledcWrite(PWMm1a, abs(speedL));
          ledcWrite(PWMm1b, 0);
          ledcWrite(PWMm2a, 0);
          ledcWrite(PWMm2b, abs(speedR));
          delay(10);
        }
        else
        {
          ledcWrite(PWMm1a, 0);
          ledcWrite(PWMm1b, abs(speedL));
          ledcWrite(PWMm2a, abs(speedR));
          ledcWrite(PWMm2b, 0);
          delay(10);
        }
        pre_yaw_error = yaw_error;
      }
      mpu.update();
    }
  }
}

void goCellFast(bool last_cell)
{

  int maxpwm = 500;
  int midpwm = 370;

  int cellCountFast = 280;

  count1 = 0;
  count2 = 0;
  mpu.update();

  float IR_prev_l = readIR2(leftIR);
  float IR_prev_r = readIR2(rightIR);

  while ((count1 + count2) / 2 < cellCountFast)
  {

    if (last_cell && ((count1 + count2) / 2) > 190)
    {
      cellCountFast = 230;
    }

    float ir_error = 0;

    float l = readIR(leftIR);
    float r = readIR(rightIR);

    if (last_cell)
    {
      if (((IR_prev_l < IR_THRESH) && (l > IR_THRESH)) || ((IR_prev_r < IR_THRESH) && (r > IR_THRESH)))
      {
        breakNow2(120);
        delay(2);
        Buzz(cellTone);
        return;
      }
    }

    if ((l < IR_THRESH) && (r < IR_THRESH))
    {
      if (l > r)
      {
        ir_error = l - ir_left_distance;
      }
      else
      {
        ir_error = ir_right_distance - r;
      }
    }

    else
    {
      if (l < IR_THRESH)
      {
        ir_error = l - ir_left_distance;
      }
      else if (r < IR_THRESH)
      {
        ir_error = ir_right_distance - r;
      }
    }
    if (abs(ir_error) < 1)
    {
      ir_error = 0;
    }

    mpu.update();
    float error = (mpu.getAngleZ() - globle_theta) + ir_error * k_ir;

    float speedR = midpwm - (kp_gyro * error + kd_gyro * (error - pre_error));
    float speedL = midpwm + (kp_gyro * error + kd_gyro * (error - pre_error));

    if (speedR < minpwm)
    {
      speedR = minpwm;
    }
    else if (speedR > maxpwm)
    {
      speedR = maxpwm;
    }
    else
    {
      speedR = (int)speedR;
    }
    if (speedL < minpwm)
    {
      speedL = minpwm;
    }
    else if (speedL > maxpwm)
    {
      speedL = maxpwm;
    }
    else
    {
      speedL = (int)speedL;
    }

    ledcWrite(PWMm1a, 0);
    ledcWrite(PWMm1b, speedL);
    ledcWrite(PWMm2a, 0);
    ledcWrite(PWMm2b, speedR);

    delay(2);
    pre_error = error;

    float f = (readTOF(frontTOF));

    if (f < 140)
    {
      float speed_front = 0;
      unsigned long time_start = millis();
      while (true)
      {
        mpu.update();
        float error_front = 0;
        float pre_error_front = 0;
        while (true)
        {
          if (abs(f - 38) < 2)
          {
            break;
          }
          if ((millis() - time_start) > ALIGN_TIME && time_start != 0)
          {
            ledcWrite(PWMm1a, 0);
            ledcWrite(PWMm1b, 0);
            ledcWrite(PWMm2a, 0);
            ledcWrite(PWMm2b, 0);
            delay(10);
            Buzz(cellTone);
            need_break = 0;
            return;
          }
          error_front = (f - 38);

          speed_front = (kp_front_tof * error_front + kd_front_tof * (error_front - pre_error_front));

          if (speed_front < -1 * maxpwm)
          {
            speed_front = -1 * maxpwm;
          }
          if (speed_front > maxpwm)
          {
            speed_front = maxpwm;
          }

          if (abs(speed_front) < minpwm)
          {
            speed_front = minpwm * (speed_front / abs(speed_front));
          }

          speed_front = (int)speed_front;

          if (speed_front > 0)
          {
            ledcWrite(PWMm1a, 0);
            ledcWrite(PWMm1b, speed_front);
            ledcWrite(PWMm2a, 0);
            ledcWrite(PWMm2b, speed_front);
          }
          else
          {
            ledcWrite(PWMm1a, -1 * speed_front);
            ledcWrite(PWMm1b, 0);
            ledcWrite(PWMm2a, -1 * speed_front);
            ledcWrite(PWMm2b, 0);
          }
          delay(2);

          pre_error_front = error_front;
          f = (readTOF(frontTOF));
        }
        f = (readTOF(frontTOF));
        if (abs(f - 38) < 2)
        {
          break;
        }
      }

      ledcWrite(PWMm1a, 0);
      ledcWrite(PWMm1b, 0);
      ledcWrite(PWMm2a, 0);
      ledcWrite(PWMm2b, 0);
      delay(10);
      Buzz(cellTone);
      return;
    }
  }
  Buzz(cellTone);
  if (last_cell)
  {
    breakNow2(breakCount);
  }
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,210);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,210);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,0);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,0);
  //    delay(10);
}

void goCellDevelIRIfBrake()
{
  int maxpwm = 400;
  int midpwm = 320;

  count1 = 0;
  count2 = 0;
  mpu.update();

  float IR_prev_l = readIR2(leftIR);
  float IR_prev_r = readIR2(rightIR);

  cellCount = 230;

  bool should_break = true;
  while ((count1 + count2) / 2 < cellCount)
  {
    float ir_error = 0;

    float l = readIR(leftIR);
    float r = readIR(rightIR);

    if ((l < IR_THRESH && r < IR_THRESH) && ((count1 + count2) / 2) > 220)
    {
      cellCount = 280;
      should_break = false;
    }

    if (((IR_prev_l < IR_THRESH) && (l > IR_THRESH)) || ((IR_prev_r < IR_THRESH) && (r > IR_THRESH)))
    {
      need_break = 0;
      breakNow2(120);
      delay(2);
      Buzz(cellTone);
      return;
    }

    if ((l < IR_THRESH) && (r < IR_THRESH))
    {
      if (l > r)
      {
        ir_error = l - ir_left_distance;
      }
      else
      {
        ir_error = ir_right_distance - r;
      }
    }

    else
    {
      if (l < IR_THRESH)
      {
        ir_error = l - ir_left_distance;
      }
      else if (r < IR_THRESH)
      {
        ir_error = ir_right_distance - r;
      }
    }
    if (abs(ir_error) < 1)
    {
      ir_error = 0;
    }

    mpu.update();
    float error = (mpu.getAngleZ() - globle_theta) + ir_error * k_ir;

    float speedR = midpwm - (kp_gyro * error + kd_gyro * (error - pre_error));
    float speedL = midpwm + (kp_gyro * error + kd_gyro * (error - pre_error));

    if (speedR < minpwm)
    {
      speedR = minpwm;
    }
    else if (speedR > maxpwm)
    {
      speedR = maxpwm;
    }
    else
    {
      speedR = (int)speedR;
    }
    if (speedL < minpwm)
    {
      speedL = minpwm;
    }
    else if (speedL > maxpwm)
    {
      speedL = maxpwm;
    }
    else
    {
      speedL = (int)speedL;
    }

    ledcWrite(PWMm1a, 0);
    ledcWrite(PWMm1b, speedL);
    ledcWrite(PWMm2a, 0);
    ledcWrite(PWMm2b, speedR);

    delay(2);
    pre_error = error;

    float f = (readTOF(frontTOF));

    if (f < 140)
    {
      float speed_front = 0;

      while (true)
      {
        mpu.update();
        float error_front = 0;
        float pre_error_front = 0;
        while (true)
        {
          if (abs(f - 38) < 2)
          {
            break;
          }

          error_front = (f - 38);

          speed_front = (kp_front_tof * error_front + kd_front_tof * (error_front - pre_error_front));

          if (speed_front < -1 * maxpwm)
          {
            speed_front = -1 * maxpwm;
          }
          if (speed_front > maxpwm)
          {
            speed_front = maxpwm;
          }

          if (abs(speed_front) < minpwm)
          {
            speed_front = minpwm * (speed_front / abs(speed_front));
          }

          speed_front = (int)speed_front;

          if (speed_front > 0)
          {
            ledcWrite(PWMm1a, 0);
            ledcWrite(PWMm1b, speed_front);
            ledcWrite(PWMm2a, 0);
            ledcWrite(PWMm2b, speed_front);
          }
          else
          {
            ledcWrite(PWMm1a, -1 * speed_front);
            ledcWrite(PWMm1b, 0);
            ledcWrite(PWMm2a, -1 * speed_front);
            ledcWrite(PWMm2b, 0);
          }
          delay(2);

          pre_error_front = error_front;
          f = (readTOF(frontTOF));
        }
        f = (readTOF(frontTOF));
        if (abs(f - 38) < 2)
        {
          break;
        }
      }

      ledcWrite(PWMm1a, 0);
      ledcWrite(PWMm1b, 0);
      ledcWrite(PWMm2a, 0);
      ledcWrite(PWMm2b, 0);
      delay(10);
      Buzz(cellTone);
      need_break = 0;
      return;
    }
  }

  Buzz(cellTone);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,210);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,210);
  //    ledcWrite(PWMm1a,0);
  //    ledcWrite(PWMm1b,0);
  //    ledcWrite(PWMm2a,0);
  //    ledcWrite(PWMm2b,0);
  //    delay(10);
  if (should_break)
  {
    breakNow2(breakCount);
  }
  need_break = 1;
}
