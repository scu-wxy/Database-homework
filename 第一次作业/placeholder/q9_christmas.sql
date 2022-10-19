with ProductName as (select distinct ProductName
		     from 'Order'
                     join OrderDetail on OrderDetail.OrderId='Order'.Id
                     join Product on Product.Id=OrderDetail.ProductId
                     join Customer on Customer.Id='Order'.CustomerId
                     where CompanyName='Queen Cozinha' and OrderDate like '2014-12-25%'
                     order by Product.Id)

select group_concat(ProductName) as ProductName from ProductName;
