#include<simplecpp>

main_program{
    initCanvas("Paint", 1500, 800);
    XEvent event;
    short lastx=0, lasty=0, linex=0, liney=0, r=0, g=0, b=0, penSize=1, dash=0, dashCount;
    bool isLine=false;
    while(true){
        nextEvent(event);
        if (mouseDragEvent(event)){
            if (dashCount==0){
                imprintLine(lastx, lasty, event.xmotion.x, event.xmotion.y, COLOR(r, g, b), penSize);
                double mag=sqrt((event.xmotion.x-lastx)*(event.xmotion.x-lastx)+(event.xmotion.y-lasty)*(event.xmotion.y-lasty)), perpX=(lasty-event.xmotion.y)/mag, perpY=(event.xmotion.x-lastx)/mag;
                for (int i=1; i<=(penSize-1)/2; i++){
                    imprintLine(lastx+i*perpX, lasty+i*perpY, event.xmotion.x+i*perpX, event.xmotion.y+i*perpY, COLOR(r, g, b));
                    imprintLine(lastx-i*perpX, lasty-i*perpY, event.xmotion.x-i*perpX, event.xmotion.y-i*perpY, COLOR(r, g, b));
                }
            }
            dashCount=(dashCount+1)%(dash+1);
            lastx=event.xmotion.x;
            lasty=event.xmotion.y;
            isLine=false;
        }
        else if (mouseButtonPressEvent(event)){
            if (isLine){
                imprintLine(lastx, lasty, event.xbutton.x, event.xbutton.y, COLOR(r, g, b), penSize);
                double mag=sqrt((event.xbutton.x-lastx)*(event.xbutton.x-lastx)+(event.xbutton.y-lasty)*(event.xbutton.y-lasty)), perpX=(lasty-event.xbutton.y)/mag, perpY=(event.xbutton.x-lastx)/mag;
                for (int i=1; i<=(penSize-1)/2; i++){
                    imprintLine(lastx+i*perpX, lasty+i*perpY, event.xbutton.x+i*perpX, event.xbutton.y+i*perpY, COLOR(r, g, b));
                    imprintLine(lastx-i*perpX, lasty-i*perpY, event.xbutton.x-i*perpX, event.xbutton.y-i*perpY, COLOR(r, g, b));
                }
                isLine=false;
            }
            else
                isLine=true;
            lastx=event.xbutton.x;
            lasty=event.xbutton.y;
        }
        else if (mouseButtonReleaseEvent(event)){
            /*if (isLine){
                imprintLine(linex, liney, event.xbutton.x, event.xbutton.y, COLOR(r, g, b));
                double mag=sqrt((event.xbutton.x-linex)*(event.xbutton.x-linex)+(event.xbutton.y-liney)*(event.xbutton.y-liney)), perpX=(liney-event.xbutton.y)/mag, perpY=(event.xbutton.x-linex)/mag;
                for (int i=1; i<=(penSize-1)/2; i++){
                    imprintLine(linex+i*perpX, liney+i*perpY, event.xbutton.x+i*perpX, event.xbutton.y+i*perpY, COLOR(r, g, b));
                    imprintLine(linex-i*perpX, liney-i*perpY, event.xbutton.x-i*perpX, event.xbutton.y-i*perpY, COLOR(r, g, b));
                }
                isLine=false;
            }
            else
                isLine=true;*/

        }
        else if (keyPressEvent(event)){
            cout<<charFromEvent(event)<<endl;
            switch (charFromEvent(event)){
                case '0': r=0;    g=0;  b=0;    break;
                case '1': r=255;    g=0;  b=0;    break;
                case '2': r=0;    g=255;  b=0;    break;
                case '3': r=0;    g=0;  b=255;    break;
                case '4': r=255;    g=130;  b=0;    break;
                case '5': r=128;    g=0;  b=128;    break;
                case '6': r=153;    g=50;  b=204;    break;
                case '7': r=223;    g=255;  b=0;    break;
                case '8': r=139;    g=69;  b=19;    break;
                case '9': r=0;    g=255;  b=255;    break;
                case '+': penSize+=2;   break;
                case '-': penSize-=penSize==1?0:2;    break;
                case '.': dash=(dash+1)%5;    break;
                case '/':{         // Case for text
                    string s="";
                    nextEvent(event);
                    while (keyPressEvent(event)){
                        s+=charFromEvent(event);
                        nextEvent(event);
                    }
                    cout<<s;
                    drawText(lastx, lasty, s, COLOR(r, g, b));
                    lastx=event.xbutton.x;
                    lasty=event.xbutton.y;
                    isLine=false;
                    break;
                }
                default:    r=255;  g=255;  b=255;  break;
            }
        }
        //cout<<"Motion: "<<event.xmotion.x<<" "<<event.xmotion.y<<"  Button: "<<event.xbutton.x<<" "<<event.xbutton.y<<"  "<<isLine<<" "<<linex<<" "<<lastx<<endl;
    }
}
