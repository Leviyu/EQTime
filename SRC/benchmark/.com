use hongyu_db1;
update EQTIME INNER JOIN tmp50
on 
tmp50.EQ_NAME = EQTIME.EQ_NAME AND
tmp50.STA = EQTIME.STA AND
tmp50.PHASE = EQTIME.PHASE 
set EQTIME.DT2 = tmp50.DT
WHERE
tmp50.EQ_NAME = EQTIME.EQ_NAME AND
tmp50.STA = EQTIME.STA AND
tmp50.PHASE = EQTIME.PHASE;