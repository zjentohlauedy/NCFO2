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
