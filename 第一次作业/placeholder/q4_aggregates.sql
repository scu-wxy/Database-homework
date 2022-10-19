select CategoryName,count(*) as sum,round(avg(UnitPrice),2),min(UnitPrice),max(UnitPrice),sum(UnitsOnOrder)
from Product 
join Category on Product.CategoryId=Category.Id
group by CategoryId
having sum>10
order by Category.Id
;
