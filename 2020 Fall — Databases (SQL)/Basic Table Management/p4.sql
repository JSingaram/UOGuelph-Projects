/*
	Execute Problem 4
*/


create or replace function q04() returns void as $$
	declare
		-- Create variables
		c1 cursor for 
			select max(t_date), customer.account, cname
			from customer 
			left join transaction 
				on customer.account = transaction.account
			left join vendor
				on transaction.vno = vendor.vno
			group by customer.account;

		account_no char(50);
		customer_name char(50);
		trans_date date;
		rec record;

	--Start executable portion of function
	begin
		open c1;

		loop
			raise notice '';

			fetch c1 into trans_date, account_no, customer_name;
			exit when not found;

			select distinct customer.account, cname, amount, vname
			into rec
			from customer 
			left join transaction 
				on customer.account = transaction.account
			left join vendor
				on transaction.vno = vendor.vno
			where (account_no = customer.account) and (trans_date = t_date);

			if ((rec.amount is null) OR (rec.vname is null)) then
				raise notice 'Customer number: %', account_no;
				raise notice 'Customer name: %', customer_name;
				raise notice 'No transaction.';
			
			else 
				raise notice 'Customer number: %', rec.account;
				raise notice 'Customer name: %', rec.cname;
				raise notice 'Amount: %', rec.amount;
				raise notice 'Vendor name: %', rec.vname;
			end if;
			
		end loop;

	-- Stop executable part of function
	end;

--Define language being used
$$ language 'plpgsql';