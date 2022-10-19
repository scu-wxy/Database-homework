select RegionDescription,FirstName,LastName,max(BirthDate)
from Employee
join EmployeeTerritory on Employee.Id=EmployeeTerritory.EmployeeId
join Territory on EmployeeTerritory.TerritoryId=Territory.Id
join Region on Region.Id=Territory.RegionId
group by Region.Id
order by Region.Id
;

