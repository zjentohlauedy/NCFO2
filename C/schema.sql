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
