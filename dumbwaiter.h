 Class DumbwaiterShaft
    with name 'dumbwaiter' 'shaft',
        short_name "dumbwaiter",
        description "It's an old dumbwaiter built into the wall here. All that's left now is the empty shaft. ",
        before [;
            receive:
                if (noun == ladder or folding_chair) print_ret(The)noun," won't fit in the dumbwaiter shaft. ";
            ],
    has container open scenery;

!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DumbwaiterShaft basement_dumbwaiter hallway_b1;

DumbwaiterShaft main_dumbwaiter hallway_m2;

DumbwaiterShaft second_floor_dumbwaiter hallway_2_2;

DumbwaiterShaft sub_basement_dumbwaiter sub_basement_01;