select Id,ShipCountry,case
	when ShipCountry in('USA','Mexico','Canada')
	then 'NorthAmerican'
	else 'OtherPlace'
	end
from 'Order'
where Id>=15445
order by Id asc
limit 20
;
