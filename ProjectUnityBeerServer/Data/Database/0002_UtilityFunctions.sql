delimiter $$

-- Function to check if a column exists
drop function if exists f_column_exists $$
create function f_column_exists(ptable text, pcolumn text)
  returns bool
  reads sql data
begin
  declare result bool;
  select
    count(*)
  into
    result
  from
    information_schema.columns
  where
    `table_name` = ptable and
    `column_name` = pcolumn;
  return result;
end $$

-- Function to check if a constraint exists
drop function if exists f_constraint_exists $$
create function f_constraint_exists(ptable text, pconstraint text)
  returns bool
  reads sql data
begin
  declare result bool;
  select
    count(*)
  into
    result
  from
    information_schema.table_constraints
  where
    `table_name` = ptable and
    `constraint_name` = pconstraint;
  return result;
end $$

-- Procedure to drop a constraint if it exists
drop procedure if exists p_add_fk $$
create procedure p_add_fk(ptable text, pconstraint text)
begin
  if not f_constraint_exists(ptable, pconstraint) then
    set @stat = concat('alter table ', ptable, ' add foreign key ', pconstraint);
    prepare pstat from @stat;
    execute pstat;
  end if;
end $$

-- Procedure to drop a constraint if it exists
drop procedure if exists p_drop_fk $$
create procedure p_drop_fk(ptable text, pconstraint text)
begin
  if f_constraint_exists(ptable, pconstraint) then
    set @stat = concat('alter table ', ptable, ' drop foreign key ', pconstraint);
    prepare pstat from @stat;
    execute pstat;
  end if;
end $$

-- Procedure to drop a key if it exists
drop procedure if exists p_add_key $$
create procedure p_add_key(ptable text, pconstraint text)
begin
  if f_constraint_exists(ptable, pconstraint) then
    set @stat = concat('alter table ', ptable, ' add key ', pconstraint);
    prepare pstat from @stat;
    execute pstat;
  end if;
end $$

-- Procedure to drop a key if it exists
drop procedure if exists p_drop_key $$
create procedure p_drop_key(ptable text, pconstraint text)
begin
  if f_constraint_exists(ptable, pconstraint) then
    set @stat = concat('alter table ', ptable, ' drop key ', pconstraint);
    prepare pstat from @stat;
    execute pstat;
  end if;
end $$

-- Procedure to add a column if it exists
drop procedure if exists p_add_column $$
create procedure p_add_column(ptable text, pcolumn text, pdesc text)
begin
  if not f_column_exists(ptable, pcolumn) then
    set @stat = concat('alter table ', ptable, ' add column ', pcolumn, ' ', pdesc );
    prepare pstat from @stat;
    execute pstat;
  end if;
end $$

-- Procedure to drop a column if it exists
drop procedure if exists p_drop_column $$
create procedure p_drop_column(ptable text, pcolumn text)
begin
  if f_column_exists(ptable, pcolumn) then
    set @stat = concat('alter table ', ptable, ' drop column ', pcolumn);
    prepare pstat from @stat;
    execute pstat;
  end if;
end $$

-- Procedure to rename a field
drop procedure if exists p_rename $$
create procedure p_rename(ptable text, poldcolumn text, pnewcolumn text, pdesc text)
begin
  if not f_column_exists(ptable, pnewcolumn) then
    set @stat = concat('alter table ', ptable, ' change ', poldcolumn, ' ', pnewcolumn, ' ', pdesc );
    prepare pstat from @stat;
    execute pstat;
  end if;
end $$

-- Function to check if the patch has been executed.
drop function if exists f_patch_executed $$
create function f_patch_executed(pid int)
  returns bool
  reads sql data
begin
  declare result bool;
  select
    count(*)
  into
    result
  from
    patches
  where
    PatchId = pid;
  return result;
end $$

-- Procedure to rename a field
drop procedure if exists p_patch_executed_successfully $$
create procedure p_patch_executed_successfully(pid int)
begin
	if not f_patch_executed( pid ) then
		insert into patches (PatchId,Updated) values( pid, now() );
		set @prnt = concat( 'select ''Patch ',pid,' executed successfully'' as ''''');
		prepare pstat from @prnt;
		execute pstat;
	end if;
end $$

delimiter ;
