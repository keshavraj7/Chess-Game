#pragma once

struct Move{
    int fromRow,fromCol,toRow,toCol;
     Move(){}
    Move(int fr,int fc,int tr,int tc):fromRow(fr),fromCol(fc),toRow(tr),toCol(tc){}
};