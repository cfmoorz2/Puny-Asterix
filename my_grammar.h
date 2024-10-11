!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   READING
Extend 'read' replace
    * noun -> Read;

[ReadSub;
    print"There's nothing worth reading on ",(the)noun,".^";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   DIALING PHONES
Verb 'dial'
    * noun -> DialObj
    * number -> DialNumber;

Verb 'hang'
    * 'up' noun -> Drop;

[DialObjSub;
    if (noun ~= handset) "I'm not sure how you can dial ",(the)noun,". ";
];

[DialNumberSub;
    if (handset notin player)   "You're not holding the phone. ";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   LOOK UNDER 
Extend 'look' first 
    * 'under' noun ->LookUnder;

[ LookUnderSub;
    "There's nothing under ",(the)noun,".";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   KNOCKING ON THINGS
Verb 'knock' 
    * noun -> Knock
    * 'on' noun -> Knock;

Extend only 'thump' 'hit' 'punch' replace
    * 'on' noun -> Knock
    * noun -> Knock;

[ KnockSub ;
    "Nothing seems to happen. ";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   SPINNING THINGS
Verb 'spin' = 'turn';

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   UNFOLD LADDER
Verb 'unfold' = 'open';

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   FLICKING SWITCHES
Verb 'flick'
    * noun -> Flick; 

[FlickSub; 
    "Nothing seems to happen. ";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   TURNING DIALS 
Extend 'turn' first
    * noun 'to' number -> TurnTo;
Extend 'turn' first
    * noun 'clockwise'/'right' 'to' number -> TurnClockwiseTo;
Extend 'turn' first
    * noun 'counterclockwise'/'left' 'to' number -> TurnCounterclockwiseTo;

[TurnClockwiseToSub; "You can't seem to turn ", (the)noun, ".";];
[TurnCounterclockwiseToSub; "You can't seem to turn ", (the)noun, ".";];
[TurnToSub; "You should specify 'clockwise/right' or 'counterclockwise/left' to a number.";];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   PLUG/UNPLUG
Verb 'plug'
    * 'in' noun -> PlugIn
    * noun 'in'/'into' noun -> PlugInto;

Verb 'unplug'
    * noun ->   UnPlug 
    * noun 'from' noun -> UnPlugFrom;

[PlugInSub ; "That doesn't seem to apply here. "; ];
[PlugIntoSub ; "That doesn't seem to apply here. "; ];
[UnPlugSub ; "That doesn't seem to apply here. "; ];
[UnPlugFromSub ; "That doesn't seem to apply here. "; ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   FLIP/TURN OVER
Verb 'flip'
    * noun -> Flip
    * 'over' noun -> Flip
    * noun 'over' -> Flip;

Extend 'turn'
    * 'over' noun -> Flip
    * noun 'over' -> Flip;

[FlipSub ; "Flipping over ",(the)noun," seems unproductive. "; ];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   FOLLOW
[FollowScope direction one_out;
    if(scope_stage == 1) rfalse;
    if(scope_stage == 3) "There doesn't seem to be follow-able. ";
    objectloop (direction in compass)
    {
        one_out = real_location.(direction.door_dir);
        !print"one_out start = ",(name)one_out,"^";
        if (one_out ofclass myDoor)
        {
            one_out = one_out.door_to(direction.door_dir);
            !print"one_out DOOR = ",(name)one_out,"^";
        } 
        if (one_out ofclass Room)
        {
            ScopeWithin(one_out);
        }
    } 
    rfalse;     
];

Verb 'follow'
    * scope=FollowScope -> Follow;

[FollowSub direction one_out; 
    !print"here noun = ",(name)noun,"^";
    if(noun in real_location) { print(The)noun," is already here.^"; rtrue; }
    objectloop (direction in compass)
    {
        one_out = real_location.(direction.door_dir);
        !print"one_out start = ",(name)one_out,"^";
        if (one_out ofclass myDoor)
        {
            one_out = one_out.door_to(direction.door_dir);
            !print"one_out DOOR = ",(name)one_out,"^";
        } 
        if (one_out ofclass Room)
        {
            if (noun in one_out) 
            {
                print"You follow ",(the)noun,;
                if (direction == u_obj) { print" upstairs.^"; <<go FAKE_U_OBJ>>;  }
                if (direction == d_obj) { print" downstairs.^"; <<go FAKE_D_OBJ>>; }
                print" to the ",(name)direction,".^^"; 
                following = true;

                switch(direction) 
                {
                    n_obj: <<go FAKE_N_OBJ>>;
                    s_obj: <<go FAKE_S_OBJ>>;
                    e_obj: <<go FAKE_E_OBJ>>;
                    w_obj: <<go FAKE_W_OBJ>>;
                    u_obj: <<go FAKE_U_OBJ>>;
                    d_obj: <<go FAKE_D_OBJ>>;
                }
            }
        }
    } 
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   CAPACITY
Verb meta 'capacity' * -> Capacity;
[ CapacitySub  i count; 
    print"You can carry a total of ",MAX_CARRIED," poorly-defined units based on weight, mass, 
    and 'awkwardness'. You are currently holding ";
    count = 0;
    objectloop(i in player) {
        count = count + i.mass;
    }
    print_ret count," units. ";    
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   SWIPING CARDS
Verb 'swipe' 'slide'
    * held -> Swipe
    * held 'through' noun -> SwipeThrough;

[ SwipeThroughSub; 
    "You can't swipe ",(the)noun," through ",(the)second,".";
];

[ SwipeSub;
    "You can't do that with ",(the)noun,".";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   PUSHING MRI BUTTONS
[mri_button ;
    if (noun == red_button or green_button) rtrue; rfalse;
];
Extend 'push' first 
    * noun=mri_button -> pushbutt;
[ PushButtSub ;
    "heh, heh...";
];

