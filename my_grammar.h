!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   READING
Extend 'read' replace
    * noun -> Read;

[ReadSub;
    print"There's nothing worth reading on ",(the)noun,".^";
];

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   SPINNING CHARTS
Verb 'spin' = 'turn';

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
!   TURNING DIALS 
Extend 'turn' first
    * noun 'to' number -> TurnTo;

[TurnToSub; "You can't seem to turn ", (the)noun, ".";];

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
    if (noun == player) "You're possessing ",(name)noun," right now.";
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
                if (direction == u_obj) { print" upstairs.^"; <<go direction>>;  }
                if (direction == d_obj) { print" downstairs.^"; <<go direction>>; }
                print" to the ",(name)direction,".^"; 
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
