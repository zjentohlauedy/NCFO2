CREATE TABLE Organizations_T
(
   Organization_Id INTEGER PRIMARY KEY,
   Name            TEXT,
   Abbreviation    TEXT,
   Season          INTEGER
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
   Home_Wins      INTEGER,
   Home_Losses    INTEGER,
   Road_Wins      INTEGER,
   Road_Losses    INTEGER,
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