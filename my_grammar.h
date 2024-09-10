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

