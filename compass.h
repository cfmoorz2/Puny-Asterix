Class   CompassDirection
  with  number 0, article "the",
        parse_name [; return -1; ]
  has   scenery;

Object Compass "compass" has concealed;

CompassDirection -> n_obj  with short_name "north",     door_dir n_to,
                                name 'n//' 'north';
CompassDirection -> s_obj  with short_name "south",     door_dir s_to,
                                name 's//' 'south';
CompassDirection -> e_obj  with short_name "east",      door_dir e_to,
                                name 'e//' 'east';
CompassDirection -> w_obj  with short_name "west",      door_dir w_to,
                                name 'w//' 'west';
CompassDirection -> u_obj  with short_name "up above",  door_dir u_to,
                                name 'u//' 'up' 'ceiling' 'above';
CompassDirection -> d_obj  with short_name "ground",    door_dir d_to,
                                name 'd//' 'down' 'floor' 'below';

