select ProductName,CompanyName,ContactName
from (select ProductName,min(OrderDate),CompanyName,ContactName 
     from (select Id,ProductName 
          from  Product 
          where Discontinued = 1) as P
	  join OrderDetail on OrderDetail.ProductId = P.Id 
 	  join 'Order' on 'Order'.Id = OrderDetail.OrderId 
          join Customer on Customer.Id = 'Order'.CustomerId
	  group by ProductName)
order by ProductName asc
;
