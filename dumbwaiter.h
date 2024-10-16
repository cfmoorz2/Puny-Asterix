 Class DumbwaiterShaft
    with name 'dumbwaiter' 'shaft',
        short_name "dumbwaiter",
        description [;
            print"It's an old dumbwaiter built into the wall here. There's a sliding metal door that 
            covers the opening. It's currently ";
            if (self has open) { 
                print"open. ";
             } else {
                print"closed. ";
            }
            "A ccable hangs down along the left and right sides of the shaft. ";
        ],
        before [;
            open:
                if (self has open)  {
                     "It's already open. ";
                } else {
                    give self open;
                    "You slide the metal door open. ";
                }
            receive:
                if (dumbwaiter_cab in self) {
                    <<insert noun dumbwaiter_cab>>;
                }   else    {
                    move noun to lost;
                    "You drop ",(the)noun," into the dumbwaiter shaft and with a clatter it disappears into the darkness. ";
                }
            search:
                if (self hasnt open)    <open self>;
                !if (children(self) == 0) "You see an empty dumbwaiter shaft. ";
            ],
        after [;
            search:
                if (children(self) == 0) "Through the panel you see an empty dumbwaiter shaft. ";
                if (dumbwaiter_cab in self) {
                    print"The dumbwaiter cab is here. ";
                    if(PrintContents("In the cab you can see ", dumbwaiter_cab)) print". ";
                    print_ret"";
                 }
            examine:
                if (self has open) {
                    if (children(self) == 0) "Through the panel you see an empty dumbwaiter shaft. ";
                if (dumbwaiter_cab in self && self has open) {
                    print"The dumbwaiter cab is here.";
                    if(PrintContents("In the cab you can see ", dumbwaiter_cab)) print". ";
                    print_ret"";
                 }
                }
        ],
    has container scenery openable ~open;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DumbwaiterShaft basement_dumbwaiter hallway_b1;
Object basement_dumbwaiter_door "dumbwaiter door" hallway_b1
    with
        parse_name [ w1 w2 ;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'dumbwaiter' && w2 == 'door') return 2;
            if (w1 == 'sliding' && w2 == 'door') return 2;
            if (w1 == 'door' or 'sliding') return 1;
        ],
        description [;
            print"It's a sliding metal door over the dumbwaiter shaft. It's currently ";
            open_or_closed(basement_dumbwaiter);
            ".";
        ],
        before [;
            open:
                <<open basement_dumbwaiter>>;
            close:
                <<close basement_dumbwaiter>>;
        ],
    has scenery;

DumbwaiterShaft main_dumbwaiter hallway_m2;
Object main_dumbwaiter_door "dumbwaiter door" hallway_m2
    with
        parse_name [ w1 w2 ;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'dumbwaiter' && w2 == 'door') return 2;
            if (w1 == 'sliding' && w2 == 'door') return 2;
            if (w1 == 'door' or 'sliding') return 1;
        ],
        description [;
            print"It's a sliding metal door over the dumbwaiter shaft. It's currently ";
            open_or_closed(main_dumbwaiter);
            ".";
        ],
        before [;
            open:
                <<open main_dumbwaiter>>;
            close:
                <<close main_dumbwaiter>>;
        ],
    has scenery;

DumbwaiterShaft second_floor_dumbwaiter hallway_2_2;
Object second_floor_dumbwaiter_door "dumbwaiter door" hallway_2_2
    with
        parse_name [ w1 w2 ;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'dumbwaiter' && w2 == 'door') return 2;
            if (w1 == 'sliding' && w2 == 'door') return 2;
            if (w1 == 'door' or 'sliding') return 1;
        ],
        description [;
            print"It's a sliding metal door over the dumbwaiter shaft. It's currently ";
            open_or_closed(second_floor_dumbwaiter);
            ".";
        ],
        before [;
            open:
                <<open second_floor_dumbwaiter>>;
            close:
                <<close second_floor_dumbwaiter>>;
        ],
    has scenery;

DumbwaiterShaft sub_basement_dumbwaiter sub_basement_01;
Object sub_basement_dumbwaiter_door "dumbwaiter door" sub_basement_01
    with
        parse_name [ w1 w2 ;
            w1 = NextWord();
            w2 = NextWord();
            if (w1 == 'dumbwaiter' && w2 == 'door') return 2;
            if (w1 == 'sliding' && w2 == 'door') return 2;
            if (w1 == 'door' or 'sliding') return 1;
        ],
        description [;
            print"It's a sliding metal door over the dumbwaiter shaft. It's currently ";
            open_or_closed(sub_basement_dumbwaiter);
            ".";
        ],
        before [;
            open:
                <<open sub_basement_dumbwaiter>>;
            close:
                <<close sub_basement_dumbwaiter>>;
        ],
    has scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object dumbwaiter_cab "dumbwaiter cab" basement_dumbwaiter
    with name 'cab',
        description [;
            print"It's the metal cab that moves up and down within the dumbwaiter shaft. ";
            if(PrintContents("In the cab you can see ", dumbwaiter_cab)) print". ";
            !if (children(self) == 0 ) "It's currently empty. "; else "";
        ],
    has container static open;

[DumbwaiterCabUp;
    switch(dumbwaiter_level)    {
        -1: if (TestScope(dumbwaiter_cab, player))  {
                print "You pull the chain and the dumbwaiter cab rises out of view.^";
                move dumbwaiter_cab to basement_dumbwaiter; 
                scope_modified = true;
                dumbwaiter_level++;
                rtrue;
            }
            move dumbwaiter_cab to basement_dumbwaiter; 
            scope_modified = true;
            dumbwaiter_level++;
            "You hear metallic grinding from within the dumbwaiter shaft. ";
        0:  if (TestScope(dumbwaiter_cab, player))  {
                print "You pull the chain and the dumbwaiter cab rises out of view.^";
                move dumbwaiter_cab to main_dumbwaiter; 
                scope_modified = true;
                dumbwaiter_level++;
                rtrue;
            }
            move dumbwaiter_cab to main_dumbwaiter; 
            scope_modified = true;
            dumbwaiter_level++;
            if(TestScope(dumbwaiter_cab, player)) "You pull the chain and the dumbwaiter cab rises into view. ";
            "You hear metallic grinding from within the dumbwaiter shaft. ";
        1:  if (TestScope(dumbwaiter_cab, player))  {
                print "You pull the chain and the dumbwaiter cab rises out of view.^";
                move dumbwaiter_cab to second_floor_dumbwaiter; 
                scope_modified = true;
                dumbwaiter_level++;
                rtrue;
            }
            move dumbwaiter_cab to second_floor_dumbwaiter; 
            scope_modified = true;
            dumbwaiter_level++;
            if(TestScope(dumbwaiter_cab, player)) "You pull the chain and the dumbwaiter cab rises into view. ";
            "You hear metallic grinding from within the dumbwaiter shaft. ";
        2:  "There's no more give in the chain. ";
        } 
];

[DumbwaiterCabDown;
    switch(dumbwaiter_level)    {
        -1:  "There's no more give in the chain. ";
        0:  if (TestScope(dumbwaiter_cab, player))  {
                print "You pull the chain and the dumbwaiter cab descends out of view.^";
                move dumbwaiter_cab to sub_basement_dumbwaiter; 
                scope_modified = true;
                dumbwaiter_level--;
                rtrue;
            }
            move dumbwaiter_cab to sub_basement_dumbwaiter; 
            scope_modified = true;
            dumbwaiter_level--;
            if(TestScope(dumbwaiter_cab, player)) "You pull the chain and the dumbwaiter descends into view. ";
            "You hear metallic grinding from within the dumbwaiter shaft. ";
        1:  if (TestScope(dumbwaiter_cab, player))  {
                print "You pull the chain and the dumbwaiter cab descends out of view.^";
                move dumbwaiter_cab to basement_dumbwaiter; 
                scope_modified = true;
                dumbwaiter_level--;
                rtrue;
            }
            move dumbwaiter_cab to basement_dumbwaiter; 
            scope_modified = true;
            dumbwaiter_level--;
            if(TestScope(dumbwaiter_cab, player)) "You pull the chain and the dumbwaiter descends into view. ";
            "You hear metallic grinding from within the dumbwaiter shaft. ";
         2:  if (TestScope(dumbwaiter_cab, player))  {
                print "You pull the chain and the dumbwaiter cab descends out of view.^";
                move dumbwaiter_cab to main_dumbwaiter; 
                scope_modified = true;
                dumbwaiter_level--;
                rtrue;
            }
            move dumbwaiter_cab to main_dumbwaiter; 
            scope_modified = true;
            dumbwaiter_level--;
            if(TestScope(dumbwaiter_cab, player)) "You pull the chain and the dumbwaiter descends into view. ";
            "You hear metallic grinding from within the dumbwaiter shaft. ";
    }
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Object left_chain "left chain"
    with name 'left' 'chain',
        description "It's a chain hanging down the left side of the dumbwaiter.",
        before [;
            pull:
                DumbwaiterCabUp();
                rtrue;
        ],
        found_in sub_basement_dumbwaiter basement_dumbwaiter main_dumbwaiter second_floor_dumbwaiter,
    has scenery;

Object right_chain "right chain"
    with name 'right' 'chain',
        description "It's a chain hanging down the right side of the dumbwaiter.",
        before [;
            pull:
                DumbwaiterCabDown();
                rtrue;
        ],
        found_in sub_basement_dumbwaiter basement_dumbwaiter main_dumbwaiter second_floor_dumbwaiter,
    has scenery;


    
