CREATE TABLE Organizations_T
(
   Organization_Id INTEGER PRIMARY KEY,
   Name            TEXT,
   Abbreviation    TEXT,
   Season          INTEGER
);

CREATE TABLE Organization_Conferences_T
(
   Organization_Id INTEGER,
   Conference_Id   INTEGER,
--
   UNIQUE ( Organization_Id, Conference_Id )
);

CREATE TABLE Conferences_T
(
   Conference_Id INTEGER PRIMARY KEY,
   Name          TEXT
);

CREATE TABLE Conference_Teams_T
(
   Conference_Id INTEGER,
   Team_Id       INTEGER,
--
   UNIQUE ( Conference_Id, Team_Id )
);

CREATE TABLE Conference_Stats_T
(
   Conference_Id  INTEGER,
   Season         INTEGER,
   Bowl_Game      INTEGER,
   Wins           INTEGER,
   Losses         INTEGER,
   Home_Wins      INTEGER,
   Home_Losses    INTEGER,
   Road_Wins      INTEGER,
   Road_Losses    INTEGER,
   Points_Scored  INTEGER,
   Points_Allowed INTEGER,
--
   UNIQUE ( Conference_Id, Season, Bowl_Game )
);

CREATE TABLE Conference_Accolades_T
(
   Conference_Id INTEGER,
   Season        INTEGER,
   Accolade      INTEGER,
--
   UNIQUE ( Conference_Id, Season, Accolade )
);

CREATE TABLE Teams_T
(
   Team_Id      INTEGER PRIMARY KEY,
   Name         TEXT,
   Location     TEXT,
   Abbreviation TEXT
);

CREATE TABLE Team_Players_T
(
   Team_Id   INTEGER,
   Season    INTEGER,
   Player_Id INTEGER,
--
   UNIQUE ( Team_Id, Season, Player_Id )
);

CREATE TABLE Team_Stats_T
(
   Team_Id        INTEGER,
   Season         INTEGER,
   Bowl_Game      INTEGER,
   Wins           INTEGER,
   Losses         INTEGER,
   Ties           INTEGER,
   Home_Wins      INTEGER,
   Home_Losses    INTEGER,
   Home_Ties      INTEGER,
   Road_Wins      INTEGER,
   Road_Losses    INTEGER,
   Road_Ties      INTEGER,
   Points_Scored  INTEGER,
   Points_Allowed INTEGER,
--
   UNIQUE ( Team_Id, Season, Bowl_Game )
);

CREATE TABLE Team_Offense_Stats_T
(
   Team_Id         INTEGER,
   Season          INTEGER,
   Bowl_Game       INTEGER,
   Pass_Attempts   INTEGER,
   Completions     INTEGER,
   Interceptions   INTEGER,
   Pass_Yards      INTEGER,
   Pass_Touchdowns INTEGER,
   Rush_Attempts   INTEGER,
   Rush_Yards      INTEGER,
   Rush_Touchdowns INTEGER,
--
   UNIQUE ( Team_Id, Season, Bowl_Game )
);

CREATE TABLE Team_Defense_Stats_T
(
   Team_Id           INTEGER,
   Season            INTEGER,
   Bowl_Game         INTEGER,
   Sacks             INTEGER,
   Interceptions     INTEGER,
   Return_Yards      INTEGER,
   Return_Touchdowns INTEGER,
   Yards_Allowed     INTEGER,
--
   UNIQUE ( Team_Id, Season, Bowl_Game )
);

CREATE TABLE Team_Kicking_Stats_T
(
   Team_Id                INTEGER,
   Season                 INTEGER,
   Bowl_Game              INTEGER,
   Extra_Point_Attempts   INTEGER,
   Extra_Points_Made      INTEGER,
   Field_Goal_Attempts    INTEGER,
   Field_Goals_Made       INTEGER,
   Punts                  INTEGER,
   Punt_Yards             INTEGER,
   Kick_Returns           INTEGER,
   Kick_Return_Yards      INTEGER,
   Kick_Return_Touchdowns INTEGER,
   Punt_Returns           INTEGER,
   Punt_Return_Yards      INTEGER,
   Punt_Return_Touchdowns INTEGER,
--
   UNIQUE ( Team_Id, Season, Bowl_Game )
);

CREATE TABLE Team_Accolades_T
(
   Team_Id  INTEGER,
   Season   INTEGER,
   Accolade INTEGER,
--
   UNIQUE ( Team_Id, Season, Accolade )
);

CREATE TABLE Players_T
(
   Player_Id       INTEGER PRIMARY KEY,
   First_Name      TEXT,
   Last_Name       TEXT,
   Face            INTEGER,
   Position        INTEGER,
   Number          INTEGER,
   Freshman_Season INTEGER,
   Maturity        INTEGER
);

CREATE TABLE Player_Ratings_T
(
   Player_Id  INTEGER PRIMARY KEY,
   Run_Speed  INTEGER,
   Rush_Power INTEGER,
   Max_Speed  INTEGER,
   Hit_Power  INTEGER
);

CREATE TABLE Player_Quarterback_Ratings_T
(
   Player_Id        INTEGER PRIMARY KEY,
   Pass_Speed       INTEGER,
   Pass_Control     INTEGER,
   Pass_Accuracy    INTEGER,
   Avoid_Pass_Block INTEGER
);

CREATE TABLE Player_Offense_Ratings_T
(
   Player_Id    INTEGER PRIMARY KEY,
   Ball_Control INTEGER,
   Receiving    INTEGER
);

CREATE TABLE Player_Defense_Ratings_T
(
   Player_Id     INTEGER PRIMARY KEY,
   Interceptions INTEGER,
   Quickness     INTEGER
);

CREATE TABLE Player_Kicking_Ratings_T
(
   Player_Id        INTEGER PRIMARY KEY,
   Kicking_Ability  INTEGER,
   Avoid_Kick_Block INTEGER
);

CREATE TABLE Player_Offense_Stats_T
(
   Player_Id            INTEGER,
   Season               INTEGER,
   Bowl_Game            INTEGER,
   Pass_Attempts        INTEGER,
   Completions          INTEGER,
   Interceptions        INTEGER,
   Pass_Yards           INTEGER,
   Pass_Touchdowns      INTEGER,
   Rush_Attempts        INTEGER,
   Rush_Yards           INTEGER,
   Rush_Touchdowns      INTEGER,
   Receptions           INTEGER,
   Receiving_Yards      INTEGER,
   Receiving_Touchdowns INTEGER,
--
   UNIQUE ( Player_Id, Season, Bowl_Game )
);

CREATE TABLE Player_Defense_Stats_T
(
   Player_Id         INTEGER,
   Season            INTEGER,
   Bowl_Game         INTEGER,
   Sacks             INTEGER,
   Interceptions     INTEGER,
   Return_Yards      INTEGER,
   Return_Touchdowns INTEGER,
--
   UNIQUE ( Player_Id, Season, Bowl_Game )
);

CREATE TABLE Player_Kicking_Stats_T
(
   Player_Id            INTEGER,
   Season               INTEGER,
   Bowl_Game            INTEGER,
   Extra_Point_Attempts INTEGER,
   Extra_Points_Made    INTEGER,
   Field_Goal_Attempts  INTEGER,
   Field_Goals_Made     INTEGER,
   Punts                INTEGER,
   Punt_Yards           INTEGER,
--
   UNIQUE ( Player_Id, Season, Bowl_Game )
);

CREATE TABLE Player_Returns_Stats_T
(
   Player_Id              INTEGER,
   Season                 INTEGER,
   Bowl_Game              INTEGER,
   Kick_Returns           INTEGER,
   Kick_Return_Yards      INTEGER,
   Kick_Return_Touchdowns INTEGER,
   Punt_Returns           INTEGER,
   Punt_Return_Yards      INTEGER,
   Punt_Return_Touchdowns INTEGER,
--
   UNIQUE ( Player_Id, Season, Bowl_Game )
);

CREATE TABLE Player_Accolades_T
(
   Player_Id INTEGER,
   Season    INTEGER,
   Accolade  INTEGER,
--
   UNIQUE ( Player_Id, Season, Accolade )
);
