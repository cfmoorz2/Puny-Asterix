 Class DumbwaiterShaft
    with name 'dumbwaiter' 'shaft',
        short_name "dumbwaiter",
        description "It's an old dumbwaiter built into the wall here. A cable hangs down along the left and right sides of the shaft. ",
        before [;
            receive:
                if (dumbwaiter_cab in self) {
                    <<insert noun dumbwaiter_cab>>;
                }   else    {
                    move noun to lost;
                    "You drop ",(the)noun," into the dumbwaiter shaft and with a clatter it disappears into the darkness. ";
                }
            ],
        after [;
            search, examine:
                !if (children(self) == 0) "You see an empty dumbwaiter shaft. ";
                if (dumbwaiter_cab in self) {
                    print"The dumbwaiter cab is here. ";
                    if(PrintContents("In the cab you can see ", dumbwaiter_cab)) ". ";
                    "The dumbwaiter cab is empty. ";
                 }
        ],
    has container open scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DumbwaiterShaft basement_dumbwaiter hallway_b1;

DumbwaiterShaft main_dumbwaiter hallway_m2;

DumbwaiterShaft second_floor_dumbwaiter hallway_2_2;

DumbwaiterShaft sub_basement_dumbwaiter sub_basement_01;


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


    
