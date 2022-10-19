select Id, OrderDate,PreviousOrderDate,round((julianday(OrderDate) - julianday(PreviousOrderDate)),2)
from (select Id,CustomerId,OrderDate,lag(OrderDate,1,OrderDate) over(order by OrderDate) as PreviousOrderDate
     from 'Order'
     where CustomerId = 'BLONP')
order by OrderDate asc
limit 10
;

