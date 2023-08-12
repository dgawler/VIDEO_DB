 PRINT "Counter......................."; COUNTER$
7150 PRINT "Category......................"; CATEG$
7160 PRINT : PRINT
7170 COLOR (6)
7180 PRINT "Is this the record you want to delete "; : INPUT RE$
7190 IF RE$ = "Y" THEN 7210
7200 GOTO 215
7210 LSET TAPENO$ = "9999": LSET MOVIENA$ = "Deleted": LSET COUNTER$ = "Dele"
7215 LSET CATEG$ = "-"
7220 PUT #1, MN
7230 GOSUB 60000
7240 PRINT : PRINT
7250 PRINT "Movie number "; MN; " has now been deleted off HARD DISK"
7260 PRINT : PRINT : PRINT : PRINT
7265 COLOR (3)
7270 PRINT "Hit <RET> to delete another movie or E to exit "; : INPUT IO$
7280 IF IO$ = "E" THEN 215
7290 GOTO 7000
7400 REM -------- STORE TIME INTO ARRAY
7410 H = VAL(HOURS$): H = H * 60
7420 M = VAL(MINUTES$)
7430 RT = H + M: TW = ((HR * 60) + MI)
7440 X = X + 1
7450 R(X) = RT - TW
7455 TAPE(X) = A
7460 RETURN
7500 REM ------ FIND BEST TAPE RETURN
7510 BEST = 1000
7520 FOR K = 0 TO X
7530 IF BEST > R(K) THEN BEST = R(K): TN1 = TAPE(K)
7540 NEXT K
7550 PRINT "Computer analysis has shown that the best tape"
7553 PRINT "for the application is "; TN1
7555 PRINT : PRINT : PRINT
7560 RETURN
8000 REM********************************************************************
8010 REM*                OPTION 7 ------- CHANGE PARTICULARS
8020 REM********************************************************************
8030 REM
8040 REM
8050 REM
8060 GOSUB 60000
8070 PRINT : PRINT : PRINT
8072 PRINT "             1.........Tape number"
8074 PRINT "             2.........Movie name"
8076 PRINT "             3.........Counter"
8078 PRINT "             4.........Category": PRINT : PRINT : PRINT : COLOR (7)
8080 PRINT "Enter the field you would like to change "; : INPUT F1$
8090 ON VAL(F1$) GOSUB 8200, 8400, 8600, 8800
8100 PRINT : PRINT "Illegal selection  ": FOR F1 = 1 TO 5000: NEXT F1
8110 GOTO 8000
8200 GOSUB 60000: REM-------------------OPTION 1
8210 PRINT : PRINT : PRINT
8220 PRINT "Enter movie number to be altered "; : INPUT MN
8230 GET #1, MN
8240 PRINT : PRINT "Current tape number is "; TAPENO$
8250 PRINT : PRINT "Do you want to change this field "; : INPUT F1$
8260 IF F1$ = "Y" THEN 8280
8270 GOTO 215
8280 PRINT : PRINT "Enter new tape number "; : INPUT T$
8290 LSET TAPENO$ = T$
8300 PUT #1, MN
8310 PRINT : PRINT "New value is "; TAPENO$
8320 PRINT : PRINT "Hit <RET> to alter another field or E to exit "; : INPUT RE$
8330 IF RE$ = "E" THEN 215
8340 GOTO 8000
8400 GOSUB 60000: REM------------------OPTION 2
8410 PRINT : PRINT : PRINT
8420 PRINT "Enter movie number to be altered "; : INPUT MN
8430 GET #1, MN
8440 PRINT : PRINT "Current movie name is "; MOVIENA$
8450 PRINT : PRINT "Do you want to change this field "; : INPUT RE$
8460 IF RE$ = "Y" THEN 8480
8470 GOTO 215
8480 PRINT : PRINT "Enter new movie name "; : INPUT T$
8485 LSET UP$ = T$: GOSUB 50000
8490 LSET MOVIENA$ = T$
8500 PUT #1, MN
8510 PRINT : PRINT "New value is "; MOVIENA$
8520 PRINT : PRINT "Hit <RET> to alter another field or E to exit "; : INPUT RE$
8530 IF RE$ = "E" THEN 215
8540 GOTO 8000
8600 GOSUB 60000: REM---------------------OPTION 3
8610 PRINT : PRINT : PRINT
8620 PRINT "Enter MOVIE number to be altered "; : INPUT MN
8630 GET #1, MN
8640 PRINT : PRINT "Current counter number is "; COUNTER$
8650 PRINT : PRINT "Do you want to change this feild "; : INPUT RE$
8660 IF RE$ = "Y" THEN 8680
8670 GOTO 215
8680 PRINT : PRINT "Enter a new counter"; : INPUT T$
8690 LSET COUNTER$ = T$
8700 PUT #1, MN
8710 PRINT : PRINT "New value is "; COUNTER$
8720 PRINT : PRINT "HIT <RET> TO ALTER ANOTHER FIELD OR E TO EXIT "; : INPUT RE$
8730 IF RE$ = "E" THEN 215
8740 GOTO 8000
8800 GOSUB 60000: REM----------------OPTION 4
8810 PRINT : PRINT : PRINT
8820 PRINT "Enter MOVIE number to be altered "; : INPUT MN
8830 GET #1, MN
8840 PRINT : PRINT "Current category is "; CATEG$
8850 PRINT : PRINT "Do you want this field changed "; : INPUT RE$
8860 IF RE$ = "Y" THEN 8880
8870 GOTO 215
8880 PRINT : PRINT "Enter new category "; : INPUT T$
8890 LSET CATEG$ = T$
8900 PUT #1, MN
8910 PRINT : PRINT "New value is "; CATEG$
8920 PRINT : PRINT "Hit <RET> to change another field or E to exit "; : INPUT RE$
8930 IF RE$ = "E" THEN 215
8940 GOTO 8000
9000 REM********************************************************************
9010 REM*                OPTION 8 -- ENTER MOVIE LOCATION                  *
9020 REM********************************************************************
9030 REM
9040 REM
9050 REM
9060 GOSUB 60000
9070 PRINT : PRINT : PRINT
9080 PRINT "Enter movie number "; : INPUT MN
9090 GET #1, MN
9100 PRINT "Movie name is "; MOVIENA$
9110 PRINT : PRINT : PRINT
9120 PRINT "Is this the movie "; : INPUT RE$
9130 IF RE$ = "Y" THEN 9150
9140 PRINT : PRINT "Hit <RET> to select another movie or E to exit "; : INPUT RE$
9142 IF RE$ = "E" THEN 215
9144 GOTO 9000
9150 PRINT : PRINT "Enter counter number "; : INPUT C$
9160 LSET COUNTER$ = C$
9170 PRINT : PRINT "Counter = "; COUNTER$
9180 PUT #1, MN
9190 PRINT : PRINT "Hit <RET> to select another movie or E to exit "; : INPUT RE$
9200 IF RE$ = "E" THEN 215
9210 GOTO 9000
10000 REM******************************************************************
10010 REM*                 OPTION 9 - PRINT MASTER FILE                   *
10020 REM******************************************************************
11000 REM
11020 L = 15
11025 GOSUB 60000
11030 PRINT "Enter page number at start of print "; : INPUT PAGE: PAGE = PAGE - 1
11031 PRINT "Starting movie # "; : INPUT V
11032 PRINT "Finishing movie # (enter 5000 for entire list) "; : INPUT V1
11040 PRINT : PRINT "Set paper to top of form "
11050 PRINT : PRINT "Press <RET> to start"; : INPUT RE$
11060 GOSUB 11500
11070 FOR A = V TO V1
11080 GET #1, A
11082 IF VAL(TAPENO$) = 0 THEN 11140
11090 L = L + 1
11100 IF L = 66 THEN GOSUB 17000
11110 LPRINT "*"; TAB(3); A; TAB(9); "*"; TAB(11); TAPENO$;
11120 LPRINT TAB(16); "*"; TAB(18); MOVIENA$; TAB(59); "*";
11130 LPRINT TAB(63); COUNTER$; TAB(69); "*"; TAB(72); CATEG$; TAB(75); "*"
11132 IF VAL(TAPENO$) = 0 THEN 11140
11134 NEXT A
11140 FOR B = 1 TO 75: LPRINT "*"; : NEXT B: LPRINT
11145 LPRINT : LPRINT : LPRINT
11150 LPRINT "TOTAL FILMS = "; A - 1
11160 GOTO 215
11500 LPRINT "VIDEO>BAS    Ver 1.0"; TAB(50); "PRINT DATE: "; DATE$
11505 PAGE = PAGE + 1
11510 LPRINT "AMSTRAD PC1640 HD30"; TAB(50); "Page number "; PAGE
11520 FOR TY = 1 TO 75: LPRINT "~"; : NEXT TY: LPRINT
11550 LPRINT : LPRINT
11560 FOR B = 1 TO 75: LPRINT "*"; : NEXT B: LPRINT
11570 LPRINT "*"; TAB(3); "MOVIE"; TAB(9); "*"; TAB(11); "TAPE";
11580 LPRINT TAB(16); "*"; TAB(34); "MOVIE NAME"; TAB(59); "*";
11590 LPRINT TAB(61); "COUNTER"; TAB(69); "*";
11600 LPRINT TAB(71); "CAT"; TAB(75); "*"
11610 FOR B = 1 TO 75: LPRINT "*"; : NEXT B: LPRINT
11620 RETURN
12000 REM*******************************************************************
12010 REM*             OPTION 10 -- STORAGE OF FILMS TO BE SEEN            *
12020 REM*******************************************************************
12030 REM
12040 GOSUB 60000
12050 PRINT "1. List movie to screen"
12060 PRINT "2. List movies to printer"
12070 PRINT "3. Enter a new movie"
12080 PRINT "4. Remove movie from list"
12090 PRINT "5. Return to menu"
12100 PRINT : PRINT : PRINT
12110 PRINT "Enter desired selection "; : INPUT RE$
12120 ON VAL(RE$) GOTO 12200, 12400, 12600, 12800, 12900
12130 GOTO 215
12200 REM-----------------OPTION 10.1
12210 REM
12220 GOSUB 60000
12230 PRINT : PRINT : PRINT
12240 FOR A = 1 TO 100
12250 GET #2, A
12255 IF LEFT$(MUSTSEE$, 1) = " " THEN 12275
12256 IF LEFT$(MUSTSEE$, 5) = "HIRED" THEN 12270
12260 PRINT MUSTSEE$
12270 NEXT A
12275 PRINT : PRINT : PRINT
12277 PRINT "Hit <RET> to continue "; : INPUT G$
12280 GOTO 12000
12400 REM-----------------OPTION 10.2
12410 REM
12420 GOSUB 60000
12430 PRINT : PRINT : PRINT
12440 PRINT "Set page at top of form and hit <RET> to continue ";
12450 INPUT A$
12460 FOR A = 1 TO 100
12470 GET #2, A
12475 LPRINT MUSTSEE$
12476 IF LEFT$(MUSTSEE$, 1) = " " THEN 12490
12477 IF LEFT$(MUSTSEE$, 5) = "HIRED" THEN 12480
12480 NEXT A
12490 GOTO 12000
12600 REM---------------- OPTION 10.3
12610 REM
12620 GOSUB 60000: PRINT : PRINT : PRINT
12622 FOR A = 1 TO 100
12624 GET #2, A
12630 IF LEFT$(MUSTSEE$, 1) = " " THEN 12660
12640 IF LEFT$(MUSTSEE$, 5) = "HIRED" THEN 12660
12650 NEXT A
12660 PRINT "Record number is "; A
12665 PRINT : PRINT : PRINT
12670 PRINT "Enter movie name "; : INPUT MN$
12680 LSET MUSTSEE$ = MN$
12685 PUT #2, A
12690 PRINT : PRINT : PRINT
12700 GOTO 12000
12800 REM -------------- OPTION 10.4
12810 GOSUB 60000: PRINT : PRINT : PRINT
12815 PRINT "Enter exact name of movie "; : INPUT RE$
12820 FOR A = 1 TO 100
12825 GET #2, A
12827 LG = LEN(RE$)
12830 IF LEFT$(MUSTSEE$, LG) = RE$ THEN 12850
12840 NEXT A
12842 PRINT : PRINT : PRINT RE$; " not found !!!!!"
12843 FOR K = 1 TO 7000: NEXT K
12844 GOTO 12000
12850 PRINT : PRINT : PRINT MUSTSEE$
12860 PRINT : PRINT "Is this the movie you want removed "; : INPUT RE$
12870 IF RE$ = "N" THEN 12000
12875 LSET MUSTSEE$ = "HIRED"
12880 PUT #2, A
12890 GOTO 12000
12900 REM ------------ OPTION 10.5
12910 REM
12920 GOTO 1000
13000 REM********************************************************************
13010 REM*            OPTION 11 -- EXIT TO DOS                              *
13020 REM********************************************************************
13030 CLOSE #1: CLOSE #2: CLOSE #3
13040 GOSUB 60000: PRINT : PRINT "Are you sure "; : INPUT RE$
13050 IF RE$ = "Y" THEN 13080
13060 GOTO 215
13070 PA$ = "OFF"
13080 SYSTEM
17000 REM **************** END OF PAGE ROUTINE **************************
17005 L = 16
17010 FOR B = 1 TO 75: LPRINT "*"; : NEXT B: LPRINT
17020 FOR B = 1 TO 7: LPRINT : NEXT B
17030 GOSUB 11500
17040 RETURN
50000 REM ------------- CHECK UPPERCASE
50010 REM
50015 UP$ = LEFT$(UP$, 1)
50020 IF ASC(UP$) > 90 THEN 50040
50030 RETURN
50040 BEEP
50050 GOSUB 60000
50060 COLOR (18)
50065 PRINT : PRINT : PRINT : PRINT
50070 PRINT "CAPS LOCK is not engaged ---- DICK-HEAD !!!!"
50080 PRINT : PRINT
50100 PRINT "Engage CAPS LOCK and hit <RET> "; : INPUT RE$
50105 SCREEN 0
50110 GOTO 215
60000 REM ------------- TITLE SCREEN WSUBROUTINE
60001 REM
60002 CLS
60005 COLOR (9)
60006 PRINT "VIDEO.BAS   Ver 1.0";
60010 PRINT TAB(50); "Current time "; TIME$
60015 PRINT "AMSTRAD PC1640 HD30";
60020 PRINT TAB(50); "Current date "; DATE$
60022 FOR P = 1 TO 78: PRINT "_"; : NEXT P: PRINT
60030 RETURN
64990 REM --------------- STARTUP WARNING REGARDING UPPERCASE
64991 REM
64992 REM
65000 CLS
65001 COLOR (18)
65002 PRINT "                            W A R N I N G "
65003 PRINT "                            ~~~~~~~~~~~~~ "
65004 BEEP
65010 COLOR (6)
65020 FOR L = 1 TO 6: PRINT : NEXT L
65030 PRINT "           For this program to operate correctly, it is"
65040 PRINT "           imperative that all entries into the files be"
65050 PRINT "           made with the CAPS LOCKS button in the ON position"
65060 PRINT
65070 PRINT "           Failure to do this will render future searches"
65080 PRINT "           inoperative unless you can remember the upper"
65090 PRINT "           and lower case characteristics of each movie name"
65100 PRINT
65110 PRINT
65120 PRINT "           Charlie & Ditty -- 20th January 1990"
65130 PRINT
65140 PRINT
65150 PRINT
65160 PRINT "Press the CAPS LOCKS button if not lit and hit <RET>"; : INPUT P9$
65170 RETURN
65200 REM ********* BACKUP DATA FILES ***************
65205 CLOSE #1, #2, #3
65210 GOSUB 60000
65220 PRINT : PRINT
65240 COLOR (6)
65250 PRINT "Insert back-up data floppy disk in drive A: & hit <RET> ";
65260 INPUT RE$
65265 CLOSE #1, #2, #3
65270 SHELL "copy c:vidat.dat a:"
65275 PRINT "VIDAT.DAT"
65280 SHELL "copy c:vidsee.dat a:"
65285 PRINT "VIDSEE.DAT"
65290 SHELL "copy c:vidrem.dat a:"
65295 PRINT "VIDREM.DAT"
65300 PRINT : PRINT
65310 PRINT "Back-up procedure complete.........."
65320 FOR T = 1 TO 8000: NEXT T
65330 GOTO 215

Ë dÈ uÍ zÏ ÇÌ äÔ í °Ú £¯ Ø˙ Õ˚ ’¸ ÿ‡ÏÙ˜ˇ)-3DV]iq&y(|ä  t                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                D L ˇˇˇˇˇˇˇˇ¯ˇˇˇˇˇˇˇˇ¯úˇˇˇˇˇˇˇ¯ôˇˇˇˇˇˇˇ¯ìˇˇˇˇˇˇˇ¯áˇˇˇˇˇˇˇ¯ìˇˇˇˇˇˇˇ¯ôˇˇˇˇˇˇˇ¯úˇˇˇˇˇˇˇ¯ˇÄ     ¯ˇøˇÑCˇÔ¯˜øﬂ (ßˇÔ¯„øè‡ ˇÔ¯¡æÛìü˝Ô¯Ä∏  ?¯Ô¯’∞ pü˙Ô¯˜∞P`N¯Ô¯„∏à‡J˙Ô¯ˇøè¿˚è¯Ô¯ˇøè¿˝á˙Ô¯ˇæ¡˚áÚo¯ˇøˇÅˇáıo¯ˇøˇ·Úo¯ˇøˇ >ıo¯ˇø˙¿Úo¯ˇøÍ∞o¯ˇøU_Ro¯ˇÉä´˛*•o¯ˇø≈UUUR¯ˇé™™™Ö¯ˇ≥ÅUUUo¯ˇºp*™®uo¯ˇáéUCÄo¯ˇáä™ /¯ˇÉÄ„P8Ä/¯ˇ°á!«Ô¯ˇ±«ÚO¯ˇ∏œ‹9O¯ˇº„¿¯5/¯ˇîÁ‹òÔ¯ˇíx«o¯ˇ∏á!«/¯ˇ†‡V8¯ˇ†Ç™é¯ˇ∞Uè¯ˇµp™™†qÔ¯ˇ≤UUTo¯ˇÖ
™™™è¯ˇÇUUUUÔ¯ˇµ*£˛™é¯ˇ≤UA«’WÔ¯ˇµG¿jøÔ¯ˇ≤ÄÄˇÔ¯ˇµÉ‡ˇÔ¯ˇ≤~<ˇÔ¯ˇµ¸ˇÔ¯ˇ≤¸Ô¯ˇ∫ˇ¯èÔ¯ˇ∏ˇé¯èÔ¯ˇ∫ˇÇê8àÓ8ˇ∏ˇ√ê0Poxˇ∫ˇœp mXˇ∏ˇ‡ x ËˇΩˇŒN~ÏˇøˇÄ ?èÓ8ˇøˇ(¢üﬂÔxˇø˛AˇÔ¯ˇÄ     ¯ˇˇˇˇˇˇˇ˘»ˇˇˇˇˇˇˇ¸»ˇˇˇˇˇˇˇ˛Hˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ˛Hˇˇˇˇˇˇˇ¸»ˇˇˇˇˇˇˇ˘»ˇˇˇˇˇˇˇˇ¯ˇˇˇˇˇˇˇˇ¯                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 ë∫)  ZAPDIR  EXE           ! !~:  ÂBASIC  EXE            (â#O„ UNFRAG  EXE            `9CïÉ SCAN    EXE           Kp€PÎ∏  VI      DOC           †Å#V⁄t  VI      EXE           $Z9 ÂBASIC  HLP            (âd˙˛ ÂBASIC  INI           R≥*t0   DISKEDITICO            0u˛  DISKEDITEXE            0v	 L       COM           œ;Cá≥G                                                                                                  Glencoe's Golden Palace k   User Defined Casino                Bally's Park Place Casino Atlantic City, New Jersey              Peppermill Hotel Casino        Reno, Nevada                     Caesar's Palace Casino  o    Las Vegas, Nevada                                                 c ûüˇˇ°¢ˇˇ§•¶ß®©™´¨≠ÆØ∞ˇˇ≤≥¥µ∂∑∏π∫ªºΩæˇˇˇˇ¡¬ˇˇƒ≈∆«»… ÀÃÕŒœ–ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ›ˇˇˇˇˇˇˇˇˇˇˇˇ‰ˇˇÊˇˇˇˇˇˇˇˇˇˇˇˇˇˇÓˇˇˇˇˇˇˇˇÛÙˇˇˇˇˇˇˇˇ˘˙˚¸ˇˇ˛ˇˇ Dear User,

Thank you for examining BlackJack!.

Even if you do not choose to become a registered user I would appreciate
hearing your comments and suggestions.  Please take a moment to complete
the following form.

Note: The program name is BlackJack!, the game in general is referred
      to as Blackjack.

                            THANK YOU FOR YOUR HELP

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
2.01d
                            BLACKJACK! Comment Form

               MAIL TO:       DONALD L. GRANGER
                              GLENCOE COMPUTING
                              1100 Highway C
                              Glencoe, Missouri
                                      63038-1404
O
P     NAME:    ______________________________________     DATE: ___/___/___
T
I     ADDRESS: ______________________________________
O
N     CITY:    ______________________________________    PHONE: __-___-____
A
L     STATE:   ______________  ZIP: _________-_______


Where did you obtain your copy of BlackJack!? ____________________________
What is the version number (displayed on the opening screen)? ____________

Hardware information (if you tried more than one setup, please list all)

Computer make and model:                        Memory: _____K

Processor (8088, 80286, 80386):

Video adapter:

Monitor:

Is program speed  __Good   __Too slow

Graphics quality is  __Excellent  __Good  __Poor

TSRs loaded while running BlackJack! (eg: Sidekick):

Program run from:  __Hard Disk  __Floppy Disk    Size:____

Did you have any problems loading or running BlackJack!? __Yes  __No
If yes, please give details.



Did you attempt to run BlackJack! from within any "operating environments"
such as Windows or DESQview?  __Yes  __No
If yes, were there any problems? (be specific)



Did you read all of the documentation? __Yes  __No

Were the rules and concepts of Blackjack clearly explained? __Yes  __No
If no, please list the improvements you would like to see:



Was the documentation on how to use the program clear? __Yes  __No
If no, please list the improvements you would like to see:




Were the Basic Strategy Tables clear? __Yes  __No
If no, how could they be improved?




Please list any features that you would like added to BlackJack!:




Please list any features that you didn't like, or think were not needed:



Your experience with casino style Blackjack  __Novice   __Average  __Expert

Do you play in Blackjack tournaments?  __Yes  __No

Have you tried other Blackjack tutorial software?  __Yes  __No
If so, which ones?

Compared to other Blackjack software please rate BlackJack!
__Great  __Good  __Average  __Fair  __Poor

Do you think the registration fee is   __Low   __Fair  __High


General comments and suggestions:





ILS                J¨C˜    VIDEO                 ≠Cä    ÂP50                 |≠CΩ                                                                    echo off
cls
echo ˇ
echo ˇ
echo ˇ
echo     Make sure your printer has at least 30 sheets of paper, and is set
echo     to the TOP of the page.
echo ˇ          ƒƒƒ
echo ˇ
echo     If you wish to cancel this operation press [Ctrl] + [C] then [Y]
echo     otherwise,
echo ˇ
pause
copy bj-docs.txt prn
copy comments.txt prn
echo ˇ
echo ˇ
echo     Printing is now complete.
echo     Please take a moment to read over the documentation.
echo     They contain useful information on Blackjack and BlackJack!.
echo ˇ
echo                      ⁄ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø
echo                      ≥ Thank you for examining BlackJack! ≥
echo                      ≥ Your support makes ShareWare work. ≥
echo                      ¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ
0'  PKZIP   EXE            ınz  PKUNZIP EXE            ı¿S  MAKESFX COM            ıÄ  ZIP2EXE EXE            ıÚ  PKZIPFIXEXE            ıﬁ"  TED     COM           Y∞∞  MAXF    EXE           „! Äñ                       ⁄ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø
                     ≥      WELCOME TO BLACKJACK!       ≥
                     ≥   = YOUR KEY TO WINNING PLAY =   ≥
                     ¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ

Please be sure that you received all of the following files
                BJ      .EXE
                BJ      .CFG
                BJ-DOCS .TXT
                CASINOS .DAT
                CARDS16 .IMG
                CARDSHGC.IMG
                COMMENTS.TXT
                SUITS16 .IMG
                SUITSHGC.IMG

If any of these files are missing please contact
                Glencoe Computing
                Donald L. Granger
                1100 Highway C
                Glencoe, MO  63038-1404

To print the documentation type COPY BJ-DOCS.TXT PRN [Enter] at the DOS prompt.
ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ

                            New features in V-2.01d

  Command Line Switches:
    BlackJack! now recognizes two command line options.
    /B - Suppress the BUST message from overwriting card images.
    /P - Suppress any re-assignments to the color palettes.

    Example: C:>BJ /B /P (switches must be seperated by at least on space)

    Note that the /P option will supercede user defined color selections.

  Display Hand Values:
    BlackJack! normally diplays the count value of the currently active
    hand.  This feature can be disabled by pressing [F4] at any time
    during casino play.

    This option is provided to offer even more accurate simulation
    of actual playing conditions.

    The default condition is ON.

  Status Box:
    The Status Box now shows the number of aces that have been played.

    This feature is added as an aid for those card counting systems which
    employ ace counting.
                         - - - - - - - - - - - - - - -
    Both the running card count and the "true count" are now displayed.
    The true count is based on the number of full decks remaining to be
    played.


  Discard Pile Graph:
    A bar graph is now displayed in the upper right corner of the casino
    screen (middle left in Count Drill).  It shows the percentage of the
    deck or shoe that has been played.  This feature is offered as an aid
    in estimating the number of cards remaining to be played.  For shoe
    games (more than two decks) yellow lines are shown indicating each deck.
    A red line is shown to indicate the re-shuffle marker.

    The graph can be toggled on and off by pressing [F5] during casino play.

    The default condition is ON

  Strategy Deviation Error
    A short beep will sound whenever a player deviates from basic strategy,
    including true count variations when "Use True Count" is active.

    The warning can be toggled on and off by pressing [F6] during casino play.

    The default c