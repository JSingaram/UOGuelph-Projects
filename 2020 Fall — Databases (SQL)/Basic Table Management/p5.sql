/*
	Execute Problem 5
*/


create or replace function q05() returns void as $$
	declare
		-- Create cursors
		c1 cursor for 
			select vendor.vno, vname, sum(amount), vbalance
			from vendor  
			left join transaction
				on vendor.vno = transaction.vno
			group by vendor.vno;
		
		-- Create variables
		vendor_no char(50);
		vendor_name char(50);
		sum_amount real;
		vendor_balance real;
		updated_balance real;

	--Start executable portion of function
	begin
		open c1;
		loop

			fetch c1 into vendor_no, vendor_name, sum_amount, vendor_balance;
			exit when not found;

			updated_balance = sum_amount + vendor_balance;

			-- If there are no transactions, the vendor will be displayed with previous balance
			if(sum_amount is null) then
				raise notice 'Vendor number: %', vendor_no;
				raise notice 'Vendor name: %', vendor_name;
				raise notice 'Updated balance: %', vendor_balance;
			else
				raise notice 'Vendor number: %', vendor_no;
				raise notice 'Vendor name: %', vendor_name;
				raise notice 'Updated balance: %', updated_balance;
			end if;

		end loop;

	-- Stop executable part of function
	end;

--Define language being used
$$ language plpgsql;