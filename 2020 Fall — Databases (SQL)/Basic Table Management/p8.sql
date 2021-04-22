/*
	Execute Problem 8
*/


create or replace function q08(input_vno char, input_account char, input_amount real) returns void as $$
	declare
	-- Create cursors
		c1 cursor for 
			select distinct vno
			from vendor;
		c2 cursor for 
			select distinct account
			from customer;

		-- Create variables
		vendor_no char(50);
		account_no char(50);
		trans_no int;
		trans_no_str char(50);
		trans_rec record;
		cust_rec record;
		vend_rec record;

	--Start executable portion of function
	begin
		open c1;
		open c2;
		raise notice'Starting program';

		loop

			fetch c1 into vendor_no;
			exit when not found;

			move backward all in c2;

			loop

				fetch c2 into account_no;
				exit when not found;

				-- Test for valid inputs
				if ((input_vno = vendor_no) and (input_account = account_no)) then
					
					-- Create transaction number 
					select count(transaction) into trans_no from transaction;
					trans_no = trans_no + 1;
					trans_no_str = 'T' || cast(trans_no as varchar);
					raise notice '%', trans_no_str;

					-- Enter transaction in table
					insert into transaction(tno, vno, account, t_date, amount)
						values (trans_no_str, vendor_no, account_no, CURRENT_DATE, input_amount);

					-- Output new transaction
					select *
					into trans_rec
					from transaction
					where (tno = trans_no_str);

					raise notice '';
					raise notice 'New Transaction';
					raise notice 'Transaction number: %', trans_rec.tno;
					raise notice 'Vendor number: %', trans_rec.vno;
					raise notice 'Account number: %', trans_rec.account;
					raise notice 'Transaction date: %', trans_rec.t_date;
					raise notice 'Amount: %', trans_rec.amount;

					-- Update customer balance
					update customer 
					set cbalance = cbalance - trans_rec.amount
					where account = trans_rec.account;

					-- Output updated customer
					select *
					into cust_rec
					from customer
					where (account = trans_rec.account);

					raise notice '';
					raise notice 'Updated Customer';
					raise notice 'Account: %', cust_rec.account;
					raise notice 'Customer name: %', cust_rec.cname;
					raise notice 'Province: %', cust_rec.province;
					raise notice 'Customer balance: %', cust_rec.cbalance;
					raise notice 'Customer limit: %', cust_rec.crlimit;

					-- Update vendor balance
					update vendor 
					set vbalance = vbalance + trans_rec.amount
					where vno = trans_rec.vno;

					-- Output updated customer 
					select *
					into vend_rec
					from vendor
					where (vno = trans_rec.vno);

					raise notice '';
					raise notice 'Updated Vendor';
					raise notice 'Vendor number: %', vend_rec.vno;
					raise notice 'Vendor name: %', vend_rec.vname;
					raise notice 'City: %', vend_rec.city;
					raise notice 'Vendor balance: %', vend_rec.vbalance;

				end if;

			end loop;

		end loop;

	-- Stop executable part of function
	end;

--Define language being used
$$ language plpgsql;