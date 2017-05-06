#include <Arduino.h>
#include <HardwareSerial.h>
#include <pins.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MOVEP 10 //P Motorspannung pro Schritt
#define MOVEI 0 //I Erhöht Motorspannung bei kleinen Fehlern
#define MOVED 0.6 //D Rechtzeitig bremsen
#define MOVEMAX 255 //25
#define MOVEIMAX 200  //
#define BOUNDS(var, max) if ((var)>(max)) (var)=(max); if ( (var)<-(max)) (var)=-(max);

void movePID( struct pins *axis, int *point, int *pointDest, struct counts *motor_cnt,int settleTime ) {

    Serial.begin(9600);
    int pointDelta, pointSpeed, pointLastDelta, pointDiff, pointInt;
    pointDelta = *pointDest - *point;
    pointLastDelta = pointInt = 0;

    while( settleTime ) {
        Serial.println(motor_cnt->cnt0);
        pointDelta = *pointDest - motor_cnt->cnt0;
        pointDiff = pointDelta - pointLastDelta;
        pointLastDelta = pointDelta;
        pointInt += pointDelta;
        BOUNDS ( pointInt , MOVEIMAX );
        pointSpeed = pointDelta * MOVEP + pointInt * MOVEI + pointDiff * MOVED;
        BOUNDS ( pointSpeed , MOVEMAX );

        if ( pointSpeed >= 0 ) {
            analogWrite(axis->right, pointSpeed);
            analogWrite(axis->left, 0);
        } else {
            analogWrite(axis->left, pointSpeed);
            analogWrite(axis->right, 0);
        }
        //if ( pointDiff * pointDelta < 5)  settleTime --;
        //if ( pointDiff < 5)  settleTime --;
        delay(10);
    }

    return;

}

#ifdef __cplusplus
}
#endif
