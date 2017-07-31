#include "ibex.h"
#include "vibes.h"
#include "tools.h"

using namespace std;
using namespace ibex;

double midPointDistance(IntervalVector X, IntervalVector Y){
    return sqrt(pow(X.mid()[0] - Y.mid()[0], 2) + pow(X.mid()[1] - Y.mid()[1], 2));
}

IntervalVector findClosest(vector<IntervalVector> listBoxes, IntervalVector boatSpeed){
    double dist = 1000000000;
    IntervalVector outputBox(2); 
    for ( int i = 0; i < listBoxes.size(); i++){
        if (midPointDistance(boatSpeed, listBoxes[i]) < dist){
            dist = midPointDistance(boatSpeed, listBoxes[i]);
            outputBox = listBoxes[i];
        }
    }
    return outputBox;
}

void pavingMov(IntervalVector speed, vector<SepInter*> listSep, vector<IntervalVector>& listBoxes){
    if (speed.is_empty()){
        return;
    }
    else if (speed.max_diam() < 0.3){
        return;
    }

    IntervalVector XinEnd(2);
    IntervalVector XoutEnd(2);
    IntervalVector Xin = speed;
    IntervalVector Xout = speed;
    IntervalVector maybeBox(2);
    vector<IntervalVector> listXout;
    vector<IntervalVector> listXin;

    //sleep(1);
    cout << endl;
    cout << "initial box : " << speed << endl;
    for (int i = 0; i < listSep.size(); i++){
        listSep[i]->SepInter::separate(Xin, Xout);
        listXout.push_back(Xout);
        listXin.push_back(Xin);
        sleep(1);
        vibes::drawBoxes({{Xin[0].lb(), Xin[0].ub(), Xin[1].lb(), Xin[1].ub()}}, "[green]");
        sleep(1);
        vibes::drawBoxes({{Xout[0].lb(), Xout[0].ub(), Xout[1].lb(), Xout[1].ub()}}, "[red]");
        cout << "separator " << i+1 << endl;
        cout << "Xin " << Xin << endl;
        cout << "Xout " << Xout << endl;
        Xin = speed;
        Xout = speed;
    }

    XoutEnd = listXout[0];
    XinEnd = listXin[0];
   //cout << "nouvelle interation" << endl; 
    for (int i = 1; i < listXout.size(); i++ ){
        //cout << "Xout " << i << " " << listXout[i] << endl;
        //cout << "Xin " << i << " " << listXin[i] << endl;
        XoutEnd = XoutEnd | listXout[i];
        XinEnd = XinEnd & listXin[i];
    }
    /*
    if (XoutEnd != XinEnd){
        cout << "___________________________________________________________________________________________________" << endl;
        cout << "Xout final " << XoutEnd << endl;
        cout << "Xin final " << XinEnd << endl;
    }
    */
    IntervalVector newBox(2);

    IntervalVector* ListComplementary;
    
    int size = XoutEnd.complementary(ListComplementary);
    
    for ( int i = 0; i < size; i++){
        newBox = ListComplementary[i]&speed;
        if (!newBox.is_flat()){
            listBoxes.push_back(newBox);
            //vibes::drawBoxes({{newBox[0].lb(), newBox[0].ub(), newBox[1].lb(), newBox[1].ub()}}, "[cyan]");
        }
    }

    
    //vibes::drawBoxes({{XoutEnd[0].lb(), XoutEnd[0].ub(), XoutEnd[1].lb(), XoutEnd[1].ub()}}, "[red]");

    //sleep(1);
      
    maybeBox = XinEnd & XoutEnd;

/*
    cout << "\n" << endl;
    cout << "Xout " << XoutEnd << endl;
    cout << "Xin " << XinEnd << endl;
    cout << "maybe " << maybeBox << endl;
    */

    //vibes::drawBoxes({{maybeBox[0].lb(), maybeBox[0].ub(), maybeBox[1].lb(), maybeBox[1].ub()}}, "[yellow]");
    //sleep(1);
    pavingMov(left(maybeBox), listSep, listBoxes);
    
    pavingMov(right(maybeBox), listSep, listBoxes);
    
}


int main(int argc, char** argv) {
    Interval T(0,30);

    vector<vector<double>> border = {{-120, 0},{0, 120},{120, 0},{0, -120}};

    double pos[2][2] ={{-1,1},{-1,1}};
    IntervalVector boatInitPos(2, pos);

    double _boatSpeed[2][2] = {{-1.4,-1.2}, {-0.5,-0.3}};
    IntervalVector boatSpeed(2,_boatSpeed);


    vector<IntervalVector> obstacles;
    double pos1[4][2] = {{2, 2.1}, {30, 31}, {-10, -9}, {2.9, 3.1}}; // {speed, posInitx, posInity, heading}
    double pos2[4][2] = {{3, 3.1}, {10, 11}, {0, 1}, {0.5, 0.6}};
    IntervalVector obs1(4, pos1);
    IntervalVector obs2(4, pos2);
    //obstacles.push_back(obs1);
    //obstacles.push_back(obs2);




    vibes::beginDrawing();
    vibes::newFigure("feasible speed");
    vibes::setFigureProperties(vibesParams("x", 100, "y", 100, "width", 800, "height", 800));


    Variable vx, vy;
    vector<SepInter*> listSep;
    Function* pf1;
    Function* pf2;
    Function* pf3;
    Function* pf4;
    SepFwdBwd* pSep1;
    SepFwdBwd* pSep2;
    SepFwdBwd* pSep3;
    SepInter* pSep;    

    
    for (int i = 0; i < border.size(); i++){
        pf1 = new Function(vx, vy, ((border[i][0] - (vx*T + boatInitPos[0]))*(border[i][1] - boatInitPos[1]) - 
                                                    (border[i][1] - (vy*T + boatInitPos[1]))*(border[i][0] - boatInitPos[0]))*
                                                    ((border[(i+1)%border.size()][0] - (vx*T + boatInitPos[0]))*(border[(i+1)%border.size()][1] - boatInitPos[1]) - 
                                                    (border[(i+1)%border.size()][1] - (vy*T + boatInitPos[1]))*(border[(i+1)%border.size()][0] - boatInitPos[0]))
                         );
        
        pf2 = new Function(vx, vy, ((border[(i+1)%border.size()][0] - border[i][0])*(border[i][1] - boatInitPos[1]) - 
                                                    (border[(i+1)%border.size()][1] - border[i][1])*(border[i][0] - boatInitPos[0]))*
                                                    ((border[(i+1)%border.size()][0] - border[i][0])*(border[i][1] - (vy*T + boatInitPos[1])) - 
                                                    (border[(i+1)%border.size()][1] - border[i][1])*(border[i][0] - (vx*T + boatInitPos[0])))
                                    );
        
        pSep1 = new SepFwdBwd(*pf1, LT);
        pSep2 = new SepFwdBwd(*pf2, LT);
        pSep = new SepInter(*pSep1, *pSep2);

        listSep.push_back(pSep);
    }
    

    for ( int i = 0; i < obstacles.size(); i++){
        pf1 = new Function(vx, vy, (vx - obstacles[i][0]*cos(obstacles[i][3]))*T + boatInitPos[0] - obstacles[i][1]);
        pf2 = new Function(vx, vy, (vy - obstacles[i][0]*sin(obstacles[i][3]))*T + boatInitPos[1] - obstacles[i][2]);
        pf3 = new Function(vx, vy, (vy - obstacles[i][0]*sin(obstacles[i][3]))*(boatInitPos[0] - obstacles[i][1]) - (vx - obstacles[i][0]*cos(obstacles[i][3]))*(boatInitPos[1] - obstacles[i][2]));

        pSep1 = new SepFwdBwd(*pf1, Interval(0,0));
        pSep2 = new SepFwdBwd(*pf2, Interval(0,0));
        pSep3 = new SepFwdBwd(*pf3, Interval(0,0));
        pSep = new SepInter(*pSep1, *pSep2, *pSep3);

        listSep.push_back(pSep);
    }


    double _speed[2][2] = {{-10, 10}, {-10, 10}};
    IntervalVector speed(2, _speed);
  
    vector<IntervalVector> listBoxes;

    pavingMov(speed, listSep, listBoxes);

    delete pf1, pf2, pf3, pf4, pSep1, pSep2, pSep3, pSep, listSep;

    /*
    vibes::drawBoxes({{boatSpeed[0].lb(), boatSpeed[0].ub(), boatSpeed[1].lb(), boatSpeed[1].ub()}}, "[red]");

    IntervalVector newSpeed = findClosest(listBoxes, boatSpeed);

    vibes::drawBoxes({{newSpeed[0].lb(), newSpeed[0].ub(), newSpeed[1].lb(), newSpeed[1].ub()}}, "[green]");
*/
    return 0;

}
