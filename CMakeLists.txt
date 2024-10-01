cmake_minimum_required(VERSION 3.14.0)
project ( slicotpp
        VERSION         0.0.1
        DESCRIPTION     "C++ wrapper for SLICOT routines"
        LANGUAGES       C CXX Fortran
        HOMEPAGE_URL    "https://gitlab.vlasovdl.ru/vlasovdl/slicotpp"
)

# Конфигурация компиляции SLICOT
find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

set ( SLICOT_FSOURCE
        SLICOT-Reference/src/AB01MD.f  SLICOT-Reference/src/MB01UW.f  SLICOT-Reference/src/MB04PA.f  SLICOT-Reference/src/SB04PX.f
        SLICOT-Reference/src/AB01ND.f  SLICOT-Reference/src/MB01UX.f  SLICOT-Reference/src/MB04PB.f  SLICOT-Reference/src/SB04PY.f
        SLICOT-Reference/src/AB01OD.f  SLICOT-Reference/src/MB01UY.f  SLICOT-Reference/src/MB04PU.f  SLICOT-Reference/src/SB04QD.f
        SLICOT-Reference/src/AB04MD.f  SLICOT-Reference/src/MB01UZ.f  SLICOT-Reference/src/MB04PY.f  SLICOT-Reference/src/SB04QR.f
        SLICOT-Reference/src/AB05MD.f  SLICOT-Reference/src/MB01VD.f  SLICOT-Reference/src/MB04QB.f  SLICOT-Reference/src/SB04QU.f
        SLICOT-Reference/src/AB05ND.f  SLICOT-Reference/src/MB01WD.f  SLICOT-Reference/src/MB04QC.f  SLICOT-Reference/src/SB04QY.f
        SLICOT-Reference/src/AB05OD.f  SLICOT-Reference/src/MB01XD.f  SLICOT-Reference/src/MB04QF.f  SLICOT-Reference/src/SB04RD.f
        SLICOT-Reference/src/AB05PD.f  SLICOT-Reference/src/MB01XY.f  SLICOT-Reference/src/MB04QS.f  SLICOT-Reference/src/SB04RV.f
        SLICOT-Reference/src/AB05QD.f  SLICOT-Reference/src/MB01YD.f  SLICOT-Reference/src/MB04QU.f  SLICOT-Reference/src/SB04RW.f
        SLICOT-Reference/src/AB05RD.f  SLICOT-Reference/src/MB01ZD.f  SLICOT-Reference/src/MB04RB.f  SLICOT-Reference/src/SB04RX.f
        SLICOT-Reference/src/AB05SD.f  SLICOT-Reference/src/MB02CD.f  SLICOT-Reference/src/MB04RD.f  SLICOT-Reference/src/SB04RY.f
        SLICOT-Reference/src/AB07MD.f  SLICOT-Reference/src/MB02CU.f  SLICOT-Reference/src/MB04RS.f  SLICOT-Reference/src/SB06ND.f
        SLICOT-Reference/src/AB07ND.f  SLICOT-Reference/src/MB02CV.f  SLICOT-Reference/src/MB04RT.f  SLICOT-Reference/src/SB08CD.f
        SLICOT-Reference/src/AB08MD.f  SLICOT-Reference/src/MB02CX.f  SLICOT-Reference/src/MB04RU.f  SLICOT-Reference/src/SB08DD.f
        SLICOT-Reference/src/AB08MZ.f  SLICOT-Reference/src/MB02CY.f  SLICOT-Reference/src/MB04RV.f  SLICOT-Reference/src/SB08ED.f
        SLICOT-Reference/src/AB08ND.f  SLICOT-Reference/src/MB02DD.f  SLICOT-Reference/src/MB04RW.f  SLICOT-Reference/src/SB08FD.f
        SLICOT-Reference/src/AB08NW.f  SLICOT-Reference/src/MB02ED.f  SLICOT-Reference/src/MB04RZ.f  SLICOT-Reference/src/SB08GD.f
        SLICOT-Reference/src/AB08NX.f  SLICOT-Reference/src/MB02FD.f  SLICOT-Reference/src/MB04SU.f  SLICOT-Reference/src/SB08HD.f
        SLICOT-Reference/src/AB08NY.f  SLICOT-Reference/src/MB02GD.f  SLICOT-Reference/src/MB04TB.f  SLICOT-Reference/src/SB08MD.f
        SLICOT-Reference/src/AB08NZ.f  SLICOT-Reference/src/MB02HD.f  SLICOT-Reference/src/MB04TS.f  SLICOT-Reference/src/SB08MY.f
        SLICOT-Reference/src/AB09AD.f  SLICOT-Reference/src/MB02ID.f  SLICOT-Reference/src/MB04TT.f  SLICOT-Reference/src/SB08ND.f
        SLICOT-Reference/src/AB09AX.f  SLICOT-Reference/src/MB02JD.f  SLICOT-Reference/src/MB04TU.f  SLICOT-Reference/src/SB08NY.f
        SLICOT-Reference/src/AB09BD.f  SLICOT-Reference/src/MB02JX.f  SLICOT-Reference/src/MB04TV.f  SLICOT-Reference/src/SB09MD.f
        SLICOT-Reference/src/AB09BX.f  SLICOT-Reference/src/MB02KD.f  SLICOT-Reference/src/MB04TW.f  SLICOT-Reference/src/SB10AD.f
        SLICOT-Reference/src/AB09CD.f  SLICOT-Reference/src/MB02MD.f  SLICOT-Reference/src/MB04TX.f  SLICOT-Reference/src/SB10DD.f
        SLICOT-Reference/src/AB09CX.f  SLICOT-Reference/src/MB02ND.f  SLICOT-Reference/src/MB04TY.f  SLICOT-Reference/src/SB10ED.f
        SLICOT-Reference/src/AB09DD.f  SLICOT-Reference/src/MB02NY.f  SLICOT-Reference/src/MB04UD.f  SLICOT-Reference/src/SB10FD.f
        SLICOT-Reference/src/AB09ED.f  SLICOT-Reference/src/MB02OD.f  SLICOT-Reference/src/MB04VD.f  SLICOT-Reference/src/SB10HD.f
        SLICOT-Reference/src/AB09FD.f  SLICOT-Reference/src/MB02PD.f  SLICOT-Reference/src/MB04VX.f  SLICOT-Reference/src/SB10ID.f
        SLICOT-Reference/src/AB09GD.f  SLICOT-Reference/src/MB02QD.f  SLICOT-Reference/src/MB04WD.f  SLICOT-Reference/src/SB10JD.f
        SLICOT-Reference/src/AB09HD.f  SLICOT-Reference/src/MB02QY.f  SLICOT-Reference/src/MB04WP.f  SLICOT-Reference/src/SB10KD.f
        SLICOT-Reference/src/AB09HX.f  SLICOT-Reference/src/MB02RD.f  SLICOT-Reference/src/MB04WR.f  SLICOT-Reference/src/SB10LD.f
        SLICOT-Reference/src/AB09HY.f  SLICOT-Reference/src/MB02RZ.f  SLICOT-Reference/src/MB04WU.f  SLICOT-Reference/src/SB10MD.f
        SLICOT-Reference/src/AB09ID.f  SLICOT-Reference/src/MB02SD.f  SLICOT-Reference/src/MB04XD.f  SLICOT-Reference/src/SB10PD.f
        SLICOT-Reference/src/AB09IX.f  SLICOT-Reference/src/MB02SZ.f  SLICOT-Reference/src/MB04XY.f  SLICOT-Reference/src/SB10QD.f
        SLICOT-Reference/src/AB09IY.f  SLICOT-Reference/src/MB02TD.f  SLICOT-Reference/src/MB04YD.f  SLICOT-Reference/src/SB10RD.f
        SLICOT-Reference/src/AB09JD.f  SLICOT-Reference/src/MB02TZ.f  SLICOT-Reference/src/MB04YW.f  SLICOT-Reference/src/SB10SD.f
        SLICOT-Reference/src/AB09JV.f  SLICOT-Reference/src/MB02UD.f  SLICOT-Reference/src/MB04ZD.f  SLICOT-Reference/src/SB10TD.f
        SLICOT-Reference/src/AB09JW.f  SLICOT-Reference/src/MB02UU.f  SLICOT-Reference/src/MB05MD.f  SLICOT-Reference/src/SB10UD.f
        SLICOT-Reference/src/AB09JX.f  SLICOT-Reference/src/MB02UV.f  SLICOT-Reference/src/MB05MY.f  SLICOT-Reference/src/SB10VD.f
        SLICOT-Reference/src/AB09KD.f  SLICOT-Reference/src/MB02UW.f  SLICOT-Reference/src/MB05ND.f  SLICOT-Reference/src/SB10WD.f
        SLICOT-Reference/src/AB09KX.f  SLICOT-Reference/src/MB02VD.f  SLICOT-Reference/src/MB05OD.f  SLICOT-Reference/src/SB10YD.f
        SLICOT-Reference/src/AB09MD.f  SLICOT-Reference/src/MB02WD.f  SLICOT-Reference/src/MB05OY.f  SLICOT-Reference/src/SB10ZD.f
        SLICOT-Reference/src/AB09ND.f  SLICOT-Reference/src/MB02XD.f  SLICOT-Reference/src/MB3JZP.f  SLICOT-Reference/src/SB10ZP.f
        SLICOT-Reference/src/AB13AD.f  SLICOT-Reference/src/MB02YD.f  SLICOT-Reference/src/MB3LZP.f  SLICOT-Reference/src/SB16AD.f
        SLICOT-Reference/src/AB13AX.f  SLICOT-Reference/src/MB03AB.f  SLICOT-Reference/src/MB3OYZ.f  SLICOT-Reference/src/SB16AY.f
        SLICOT-Reference/src/AB13BD.f  SLICOT-Reference/src/MB03AD.f  SLICOT-Reference/src/MB3PYZ.f  SLICOT-Reference/src/SB16BD.f
        SLICOT-Reference/src/AB13CD.f  SLICOT-Reference/src/MB03AE.f  SLICOT-Reference/src/MB4DBZ.f  SLICOT-Reference/src/SB16CD.f
        SLICOT-Reference/src/AB13DD.f  SLICOT-Reference/src/MB03AF.f  SLICOT-Reference/src/MB4DLZ.f  SLICOT-Reference/src/SB16CY.f
        SLICOT-Reference/src/AB13DX.f  SLICOT-Reference/src/MB03AG.f  SLICOT-Reference/src/MB4DPZ.f  SLICOT-Reference/src/select.f
        SLICOT-Reference/src/AB13ED.f  SLICOT-Reference/src/MB03AH.f  SLICOT-Reference/src/MC01MD.f  SLICOT-Reference/src/SG02AD.f
        SLICOT-Reference/src/AB13FD.f  SLICOT-Reference/src/MB03AI.f  SLICOT-Reference/src/MC01ND.f  SLICOT-Reference/src/SG02CV.f
        SLICOT-Reference/src/AB13HD.f  SLICOT-Reference/src/MB03BA.f  SLICOT-Reference/src/MC01OD.f  SLICOT-Reference/src/SG02CW.f
        SLICOT-Reference/src/AB13ID.f  SLICOT-Reference/src/MB03BB.f  SLICOT-Reference/src/MC01PD.f  SLICOT-Reference/src/SG02CX.f
        SLICOT-Reference/src/AB13MD.f  SLICOT-Reference/src/MB03BC.f  SLICOT-Reference/src/MC01PY.f  SLICOT-Reference/src/SG02ND.f
        SLICOT-Reference/src/AB8NXZ.f  SLICOT-Reference/src/MB03BD.f  SLICOT-Reference/src/MC01QD.f  SLICOT-Reference/src/SG03AD.f
        SLICOT-Reference/src/AG07BD.f  SLICOT-Reference/src/MB03BE.f  SLICOT-Reference/src/MC01RD.f  SLICOT-Reference/src/SG03AX.f
        SLICOT-Reference/src/AG08BD.f  SLICOT-Reference/src/MB03BF.f  SLICOT-Reference/src/MC01SD.f  SLICOT-Reference/src/SG03AY.f
        SLICOT-Reference/src/AG08BY.f  SLICOT-Reference/src/MB03BG.f  SLICOT-Reference/src/MC01SW.f  SLICOT-Reference/src/SG03BD.f
        SLICOT-Reference/src/AG08BZ.f  SLICOT-Reference/src/MB03BZ.f  SLICOT-Reference/src/MC01SX.f  SLICOT-Reference/src/SG03BR.f
        SLICOT-Reference/src/AG8BYZ.f  SLICOT-Reference/src/MB03CD.f  SLICOT-Reference/src/MC01SY.f  SLICOT-Reference/src/SG03BS.f
        SLICOT-Reference/src/BB01AD.f  SLICOT-Reference/src/MB03CZ.f  SLICOT-Reference/src/MC01TD.f  SLICOT-Reference/src/SG03BT.f
        SLICOT-Reference/src/BB02AD.f  SLICOT-Reference/src/MB03DD.f  SLICOT-Reference/src/MC01VD.f  SLICOT-Reference/src/SG03BU.f
        SLICOT-Reference/src/BB03AD.f  SLICOT-Reference/src/MB03DZ.f  SLICOT-Reference/src/MC01WD.f  SLICOT-Reference/src/SG03BV.f
        SLICOT-Reference/src/BB04AD.f  SLICOT-Reference/src/MB03ED.f  SLICOT-Reference/src/MC01XD.f  SLICOT-Reference/src/SG03BW.f
        SLICOT-Reference/src/BD01AD.f  SLICOT-Reference/src/MB03FD.f  SLICOT-Reference/src/MC03MD.f  SLICOT-Reference/src/SG03BX.f
        SLICOT-Reference/src/BD02AD.f  SLICOT-Reference/src/MB03FZ.f  SLICOT-Reference/src/MC03ND.f  SLICOT-Reference/src/SG03BY.f
        SLICOT-Reference/src/DE01OD.f  SLICOT-Reference/src/MB03GD.f  SLICOT-Reference/src/MC03NX.f  SLICOT-Reference/src/SG03BZ.f
        SLICOT-Reference/src/DE01PD.f  SLICOT-Reference/src/MB03GZ.f  SLICOT-Reference/src/MC03NY.f  SLICOT-Reference/src/SLCT_DLATZM.f
        SLICOT-Reference/src/delctg.f  SLICOT-Reference/src/MB03HD.f  SLICOT-Reference/src/MD03AD.f  SLICOT-Reference/src/SLCT_ZLATZM.f
        SLICOT-Reference/src/DF01MD.f  SLICOT-Reference/src/MB03HZ.f  SLICOT-Reference/src/MD03BA.f  SLICOT-Reference/src/TB01ID.f
        SLICOT-Reference/src/DG01MD.f  SLICOT-Reference/src/MB03ID.f  SLICOT-Reference/src/MD03BB.f  SLICOT-Reference/src/TB01IZ.f
        SLICOT-Reference/src/DG01ND.f  SLICOT-Reference/src/MB03IZ.f  SLICOT-Reference/src/MD03BD.f  SLICOT-Reference/src/TB01KD.f
        SLICOT-Reference/src/DG01NY.f  SLICOT-Reference/src/MB03JD.f  SLICOT-Reference/src/MD03BF.f  SLICOT-Reference/src/TB01KX.f
        SLICOT-Reference/src/DG01OD.f  SLICOT-Reference/src/MB03JP.f  SLICOT-Reference/src/MD03BX.f  SLICOT-Reference/src/TB01LD.f
        SLICOT-Reference/src/DK01MD.f  SLICOT-Reference/src/MB03JZ.f  SLICOT-Reference/src/MD03BY.f  SLICOT-Reference/src/TB01MD.f
        SLICOT-Reference/src/FB01QD.f  SLICOT-Reference/src/MB03KA.f  SLICOT-Reference/src/NF01AD.f  SLICOT-Reference/src/TB01ND.f
        SLICOT-Reference/src/FB01RD.f  SLICOT-Reference/src/MB03KB.f  SLICOT-Reference/src/NF01AY.f  SLICOT-Reference/src/TB01PD.f
        SLICOT-Reference/src/FB01SD.f  SLICOT-Reference/src/MB03KC.f  SLICOT-Reference/src/NF01BA.f  SLICOT-Reference/src/TB01PX.f
        SLICOT-Reference/src/FB01TD.f  SLICOT-Reference/src/MB03KD.f  SLICOT-Reference/src/NF01BB.f  SLICOT-Reference/src/TB01TD.f
        SLICOT-Reference/src/FB01VD.f  SLICOT-Reference/src/MB03KE.f  SLICOT-Reference/src/NF01BD.f  SLICOT-Reference/src/TB01TY.f
        SLICOT-Reference/src/FD01AD.f  SLICOT-Reference/src/MB03LD.f  SLICOT-Reference/src/NF01BE.f  SLICOT-Reference/src/TB01UD.f
        SLICOT-Reference/src/IB01AD.f  SLICOT-Reference/src/MB03LF.f  SLICOT-Reference/src/NF01BF.f  SLICOT-Reference/src/TB01UX.f
        SLICOT-Reference/src/IB01BD.f  SLICOT-Reference/src/MB03LP.f  SLICOT-Reference/src/NF01BP.f  SLICOT-Reference/src/TB01UY.f
        SLICOT-Reference/src/IB01CD.f  SLICOT-Reference/src/MB03LZ.f  SLICOT-Reference/src/NF01BQ.f  SLICOT-Reference/src/TB01VD.f
        SLICOT-Reference/src/IB01MD.f  SLICOT-Reference/src/MB03MD.f  SLICOT-Reference/src/NF01BR.f  SLICOT-Reference/src/TB01VY.f
        SLICOT-Reference/src/IB01MY.f  SLICOT-Reference/src/MB03MY.f  SLICOT-Reference/src/NF01BS.f  SLICOT-Reference/src/TB01WD.f
        SLICOT-Reference/src/IB01ND.f  SLICOT-Reference/src/MB03ND.f  SLICOT-Reference/src/NF01BU.f  SLICOT-Reference/src/TB01WX.f
        SLICOT-Reference/src/IB01OD.f  SLICOT-Reference/src/MB03NY.f  SLICOT-Reference/src/NF01BV.f  SLICOT-Reference/src/TB01XD.f
        SLICOT-Reference/src/IB01OY.f  SLICOT-Reference/src/MB03OD.f  SLICOT-Reference/src/NF01BW.f  SLICOT-Reference/src/TB01XZ.f
        SLICOT-Reference/src/IB01PD.f  SLICOT-Reference/src/MB03OY.f  SLICOT-Reference/src/NF01BX.f  SLICOT-Reference/src/TB01YD.f
        SLICOT-Reference/src/IB01PX.f  SLICOT-Reference/src/MB03PD.f  SLICOT-Reference/src/NF01BY.f  SLICOT-Reference/src/TB01ZD.f
        SLICOT-Reference/src/IB01PY.f  SLICOT-Reference/src/MB03PY.f  SLICOT-Reference/src/SB01BD.f  SLICOT-Reference/src/TB03AD.f
        SLICOT-Reference/src/IB01QD.f  SLICOT-Reference/src/MB03QD.f  SLICOT-Reference/src/SB01BX.f  SLICOT-Reference/src/TB03AY.f
        SLICOT-Reference/src/IB01RD.f  SLICOT-Reference/src/MB03QG.f  SLICOT-Reference/src/SB01BY.f  SLICOT-Reference/src/TB04AD.f
        SLICOT-Reference/src/IB03AD.f  SLICOT-Reference/src/MB03QV.f  SLICOT-Reference/src/SB01DD.f  SLICOT-Reference/src/TB04AY.f
        SLICOT-Reference/src/IB03BD.f  SLICOT-Reference/src/MB03QW.f  SLICOT-Reference/src/SB01FY.f  SLICOT-Reference/src/TB04BD.f
        SLICOT-Reference/src/MA01AD.f  SLICOT-Reference/src/MB03QX.f  SLICOT-Reference/src/SB01MD.f  SLICOT-Reference/src/TB04BV.f
        SLICOT-Reference/src/MA01BD.f  SLICOT-Reference/src/MB03QY.f  SLICOT-Reference/src/SB02CX.f  SLICOT-Reference/src/TB04BW.f
        SLICOT-Reference/src/MA01BZ.f  SLICOT-Reference/src/MB03RD.f  SLICOT-Reference/src/SB02MD.f  SLICOT-Reference/src/TB04BX.f
        SLICOT-Reference/src/MA01CD.f  SLICOT-Reference/src/MB03RW.f  SLICOT-Reference/src/SB02MR.f  SLICOT-Reference/src/TB04CD.f
        SLICOT-Reference/src/MA01DD.f  SLICOT-Reference/src/MB03RX.f  SLICOT-Reference/src/SB02MS.f  SLICOT-Reference/src/TB05AD.f
        SLICOT-Reference/src/MA01DZ.f  SLICOT-Reference/src/MB03RY.f  SLICOT-Reference/src/SB02MT.f  SLICOT-Reference/src/TC01OD.f
        SLICOT-Reference/src/MA02AD.f  SLICOT-Reference/src/MB03RZ.f  SLICOT-Reference/src/SB02MU.f  SLICOT-Reference/src/TC04AD.f
        SLICOT-Reference/src/MA02AZ.f  SLICOT-Reference/src/MB03SD.f  SLICOT-Reference/src/SB02MV.f  SLICOT-Reference/src/TC05AD.f
        SLICOT-Reference/src/MA02BD.f  SLICOT-Reference/src/MB03TD.f  SLICOT-Reference/src/SB02MW.f  SLICOT-Reference/src/TD03AD.f
        SLICOT-Reference/src/MA02BZ.f  SLICOT-Reference/src/MB03TS.f  SLICOT-Reference/src/SB02MX.f  SLICOT-Reference/src/TD03AY.f
        SLICOT-Reference/src/MA02CD.f  SLICOT-Reference/src/MB03UD.f  SLICOT-Reference/src/SB02ND.f  SLICOT-Reference/src/TD04AD.f
        SLICOT-Reference/src/MA02CZ.f  SLICOT-Reference/src/MB03VD.f  SLICOT-Reference/src/SB02OD.f  SLICOT-Reference/src/TD05AD.f
        SLICOT-Reference/src/MA02DD.f  SLICOT-Reference/src/MB03VW.f  SLICOT-Reference/src/SB02OU.f  SLICOT-Reference/src/TF01MD.f
        SLICOT-Reference/src/MA02ED.f  SLICOT-Reference/src/MB03VY.f  SLICOT-Reference/src/SB02OV.f  SLICOT-Reference/src/TF01MX.f
        SLICOT-Reference/src/MA02ES.f  SLICOT-Reference/src/MB03WA.f  SLICOT-Reference/src/SB02OW.f  SLICOT-Reference/src/TF01MY.f
        SLICOT-Reference/src/MA02EZ.f  SLICOT-Reference/src/MB03WD.f  SLICOT-Reference/src/SB02OX.f  SLICOT-Reference/src/TF01ND.f
        SLICOT-Reference/src/MA02FD.f  SLICOT-Reference/src/MB03WX.f  SLICOT-Reference/src/SB02OY.f  SLICOT-Reference/src/TF01OD.f
        SLICOT-Reference/src/MA02GD.f  SLICOT-Reference/src/MB03XD.f  SLICOT-Reference/src/SB02PD.f  SLICOT-Reference/src/TF01PD.f
        SLICOT-Reference/src/MA02GZ.f  SLICOT-Reference/src/MB03XP.f  SLICOT-Reference/src/SB02QD.f  SLICOT-Reference/src/TF01QD.f
        SLICOT-Reference/src/MA02HD.f  SLICOT-Reference/src/MB03XS.f  SLICOT-Reference/src/SB02RD.f  SLICOT-Reference/src/TF01RD.f
        SLICOT-Reference/src/MA02HZ.f  SLICOT-Reference/src/MB03XU.f  SLICOT-Reference/src/SB02RU.f  SLICOT-Reference/src/TG01AD.f
        SLICOT-Reference/src/MA02ID.f  SLICOT-Reference/src/MB03XZ.f  SLICOT-Reference/src/SB02SD.f  SLICOT-Reference/src/TG01AZ.f
        SLICOT-Reference/src/MA02IZ.f  SLICOT-Reference/src/MB03YA.f  SLICOT-Reference/src/SB03MD.f  SLICOT-Reference/src/TG01BD.f
        SLICOT-Reference/src/MA02JD.f  SLICOT-Reference/src/MB03YD.f  SLICOT-Reference/src/SB03MU.f  SLICOT-Reference/src/TG01CD.f
        SLICOT-Reference/src/MA02JZ.f  SLICOT-Reference/src/MB03YT.f  SLICOT-Reference/src/SB03MV.f  SLICOT-Reference/src/TG01DD.f
        SLICOT-Reference/src/MA02MD.f  SLICOT-Reference/src/MB03ZA.f  SLICOT-Reference/src/SB03MW.f  SLICOT-Reference/src/TG01ED.f
        SLICOT-Reference/src/MA02MZ.f  SLICOT-Reference/src/MB03ZD.f  SLICOT-Reference/src/SB03MX.f  SLICOT-Reference/src/TG01FD.f
        SLICOT-Reference/src/MA02NZ.f  SLICOT-Reference/src/MB04AD.f  SLICOT-Reference/src/SB03MY.f  SLICOT-Reference/src/TG01FZ.f
        SLICOT-Reference/src/MA02OD.f  SLICOT-Reference/src/MB04AZ.f  SLICOT-Reference/src/SB03OD.f  SLICOT-Reference/src/TG01GD.f
        SLICOT-Reference/src/MA02OZ.f  SLICOT-Reference/src/MB04BD.f  SLICOT-Reference/src/SB03OR.f  SLICOT-Reference/src/TG01HD.f
        SLICOT-Reference/src/MA02PD.f  SLICOT-Reference/src/MB04BP.f  SLICOT-Reference/src/SB03OS.f  SLICOT-Reference/src/TG01HU.f
        SLICOT-Reference/src/MA02PZ.f  SLICOT-Reference/src/MB04BZ.f  SLICOT-Reference/src/SB03OT.f  SLICOT-Reference/src/TG01HX.f
        SLICOT-Reference/src/MA02RD.f  SLICOT-Reference/src/MB04CD.f  SLICOT-Reference/src/SB03OU.f  SLICOT-Reference/src/TG01HY.f
        SLICOT-Reference/src/MA02SD.f  SLICOT-Reference/src/MB04DB.f  SLICOT-Reference/src/SB03OV.f  SLICOT-Reference/src/TG01ID.f
        SLICOT-Reference/src/MB01KD.f  SLICOT-Reference/src/MB04DD.f  SLICOT-Reference/src/SB03OY.f  SLICOT-Reference/src/TG01JD.f
        SLICOT-Reference/src/MB01LD.f  SLICOT-Reference/src/MB04DI.f  SLICOT-Reference/src/SB03OZ.f  SLICOT-Reference/src/TG01JY.f
        SLICOT-Reference/src/MB01MD.f  SLICOT-Reference/src/MB04DL.f  SLICOT-Reference/src/SB03PD.f  SLICOT-Reference/src/TG01KD.f
        SLICOT-Reference/src/MB01ND.f  SLICOT-Reference/src/MB04DP.f  SLICOT-Reference/src/SB03QD.f  SLICOT-Reference/src/TG01KZ.f
        SLICOT-Reference/src/MB01OC.f  SLICOT-Reference/src/MB04DS.f  SLICOT-Reference/src/SB03QX.f  SLICOT-Reference/src/TG01LD.f
        SLICOT-Reference/src/MB01OD.f  SLICOT-Reference/src/MB04DY.f  SLICOT-Reference/src/SB03QY.f  SLICOT-Reference/src/TG01LY.f
        SLICOT-Reference/src/MB01OE.f  SLICOT-Reference/src/MB04DZ.f  SLICOT-Reference/src/SB03RD.f  SLICOT-Reference/src/TG01MD.f
        SLICOT-Reference/src/MB01OH.f  SLICOT-Reference/src/MB04ED.f  SLICOT-Reference/src/SB03SD.f  SLICOT-Reference/src/TG01ND.f
        SLICOT-Reference/src/MB01OO.f  SLICOT-Reference/src/MB04FD.f  SLICOT-Reference/src/SB03SX.f  SLICOT-Reference/src/TG01NX.f
        SLICOT-Reference/src/MB01OS.f  SLICOT-Reference/src/MB04FP.f  SLICOT-Reference/src/SB03SY.f  SLICOT-Reference/src/TG01OA.f
        SLICOT-Reference/src/MB01OT.f  SLICOT-Reference/src/MB04GD.f  SLICOT-Reference/src/SB03TD.f  SLICOT-Reference/src/TG01OB.f
        SLICOT-Reference/src/MB01PD.f  SLICOT-Reference/src/MB04HD.f  SLICOT-Reference/src/SB03UD.f  SLICOT-Reference/src/TG01OD.f
        SLICOT-Reference/src/MB01QD.f  SLICOT-Reference/src/MB04ID.f  SLICOT-Reference/src/SB04MD.f  SLICOT-Reference/src/TG01OZ.f
        SLICOT-Reference/src/MB01RB.f  SLICOT-Reference/src/MB04IY.f  SLICOT-Reference/src/SB04MR.f  SLICOT-Reference/src/TG01PD.f
        SLICOT-Reference/src/MB01RD.f  SLICOT-Reference/src/MB04IZ.f  SLICOT-Reference/src/SB04MU.f  SLICOT-Reference/src/TG01QD.f
        SLICOT-Reference/src/MB01RH.f  SLICOT-Reference/src/MB04JD.f  SLICOT-Reference/src/SB04MW.f  SLICOT-Reference/src/TG01WD.f
        SLICOT-Reference/src/MB01RT.f  SLICOT-Reference/src/MB04KD.f  SLICOT-Reference/src/SB04MY.f  SLICOT-Reference/src/UD01BD.f
        SLICOT-Reference/src/MB01RU.f  SLICOT-Reference/src/MB04LD.f  SLICOT-Reference/src/SB04ND.f  SLICOT-Reference/src/UD01CD.f
        SLICOT-Reference/src/MB01RW.f  SLICOT-Reference/src/MB04MD.f  SLICOT-Reference/src/SB04NV.f  SLICOT-Reference/src/UD01DD.f
        SLICOT-Reference/src/MB01RX.f  SLICOT-Reference/src/MB04ND.f  SLICOT-Reference/src/SB04NW.f  SLICOT-Reference/src/UD01MD.f
        SLICOT-Reference/src/MB01RY.f  SLICOT-Reference/src/MB04NY.f  SLICOT-Reference/src/SB04NX.f  SLICOT-Reference/src/UD01MZ.f
        SLICOT-Reference/src/MB01SD.f  SLICOT-Reference/src/MB04OD.f  SLICOT-Reference/src/SB04NY.f  SLICOT-Reference/src/UD01ND.f
        SLICOT-Reference/src/MB01SS.f  SLICOT-Reference/src/MB04OW.f  SLICOT-Reference/src/SB04OD.f  SLICOT-Reference/src/UE01MD.f
        SLICOT-Reference/src/MB01TD.f  SLICOT-Reference/src/MB04OX.f  SLICOT-Reference/src/SB04OW.f  SLICOT-Reference/src/zelctg.f
        SLICOT-Reference/src/MB01UD.f  SLICOT-Reference/src/MB04OY.f  SLICOT-Reference/src/SB04PD.f
)

add_library(slicot OBJECT
        ${SLICOT_FSOURCE}
        src/XERBLA.f
        src/ftruefalse.f
)
target_link_libraries(slicot PUBLIC ${LAPACK_LIBRARIES})

# Конфигурация компиляции SLICOTPP
add_library(slicotpp INTERFACE
)
target_include_directories(slicotpp INTERFACE ./include)
target_link_libraries(slicotpp INTERFACE slicot)

# Конфигурация тестов
# По умолчанию тесты не строятся
# При необходимости построить тесты нужно на этапе конфигурации указать
# -DVLASOVDL_SLICOTPP_BUILD_TEST=ON
set(VLASOVDL_SLICOTPP_BUILD_TEST OFF CACHE BOOL "The need to build tests")
if(VLASOVDL_SLICOTPP_BUILD_TEST)
add_subdirectory(test bin/test)
endif()