select CompanyName,CustomerId,TotalExpenditures
from (select*,ntile(4) over(order by TotalExpenditures) as result
     from (select ifnull(CompanyName,'MISSING_NAME') as CompanyName,CustomerId,round(sum(UnitPrice*Quantity),2) as TotalExpenditures
          from 'Order'
     	       join OrderDetail on OrderDetail.OrderId='Order'.Id
               left join Customer on 'Order'.CustomerId=Customer.Id
          group by CustomerId))
where result=1
order by TotalExpenditures asc
;

