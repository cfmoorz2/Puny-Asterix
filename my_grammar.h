!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   READING
Extend 'read' replace
    * noun -> Read;

[ReadSub;
    print"There's nothing worth reading on ",(the)noun,".^";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   DIALING PHONES
Verb 'dial' 'call'
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
!   CLOSE LADDER
Verb 'fold' 'collapse' = 'close';

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
Extend 'set' first
    * noun 'to' number -> TurnTo;

[TurnToSub; "That doesn't seem to apply here.";];

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
    if(scope_stage == 3) "That doesn't seem to be follow-able. ";
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
    and 'awkwardness'. Worn items do not count against your total. You are currently holding ";
    count = 0;
    objectloop(i in player && i hasnt worn) {
        count = count + i.mass;
    }
    print count," units.^"; 
    if(backpack in player)
    {
        print"^Your backpack can hold 20 poorly-defined units and is currently holding ";
        print_ret backpack.is_holding()," units. ";  
    } 
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
!   untie the balloon
Verb 'untie' 
    * noun -> Untie
    * noun 'from' noun -> Untie;

[ UntieSub; 
    "That doesn't seem to apple here. ";   
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   HELP
Verb meta 'help'  * -> Help;
[ HelpSub ;
    "Find out who killed Sid Jorry and get your signature form signed along the way...^
    Extra commands available:^
    @@8'capacity' (helpful with inventory management)^
    @@8'follow x'^ 
    @@8'talk to x'";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   WRITE & SIGN
Verb 'write' 
    * 'on' noun -> Write
    * 'on' noun 'with' held -> Write;

[ writesub;
    if (second ~= 0 && second ~= pen) "You can't write with ",(the)second,". ";
    "You shouldn't deface ",(the)noun,". ";
];

Verb 'sign'
    * noun -> Sign
    * noun 'with' held -> Sign;

[ signsub ;
    if (second ~= 0 && second ~= pen) "You can't write with ",(the)second,". ";
    "You shouldn't deface ",(the)noun,". ";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   PET
Verb 'pet'
    * creature -> Pet;

[ petsub;
    "We don't want to lose our PG rating. ";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   UNLOCK & LOCK

Extend 'unlock' first
    * noun -> myunlock;

[ myunlocksub ;
    "That doesn't seem to be possible. ";
];

Extend 'lock' first
    * noun -> mylock;

[ mylocksub ;
    "That doesn't seem to be possible. ";
];