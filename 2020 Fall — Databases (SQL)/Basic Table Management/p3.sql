/*
	Execute Problem 3
*/


create or replace function q03(cust_account char, cust_name char, cust_province char, cust_limit int) returns void as $$
	declare

	--Start executable portion of function
	begin
		insert into customer(account, cname, province, cbalance, crlimit)
			values(cust_account, cust_name, cust_province, 0.00, cust_limit);

	-- Stop executable part of function
	end;

--Define language being used
$$ language plpgsql;