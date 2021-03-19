from ResultsSet import *


rs = ResultsSet()

print("|============================================|")
print("|== Test functionality when rows are empty ==|")
print("|============================================|")
print("::GetRow(0) returned '{0}' - expected 'None'".format(rs.GetRow(12)))
print("::Size() returned '{0}' - expected '0'".format(rs.Size()))
print("::First() returned '{0}' - expected 'None'".format(rs.First()))
print("::Next() returned '{0}' - expected 'None'".format(rs.Next()))
print("::Last() returned '{0}' - expected 'None'".format(rs.Last()))
rs.Clear()

print("|========================================|")
print("|== Test functionality with only 1 row ==|")
print("|========================================|")
rs.Add('Test #1')
print("::GetRow(0) returned '{0}' - expected 'Test #1'".format(rs.GetRow(0)))
print("::Size() returned '{0}' - expected '1'".format(rs.Size()))
print("::First() returned '{0}' - expected 'Test #1'".format(rs.First()))
print("::Next() returned '{0}' - expected 'None'".format(rs.Next()))
print("::Last() returned '{0}' - expected 'Test #1'".format(rs.Last()))

print("|====================================|")
print("|== Test functionality with 2 rows ==|")
print("|====================================|")
rs.Add('Test #2')
print("::GetRow(1) returned '{0}' - expected 'Test #1'".format(rs.GetRow(1)))
print("::Size() returned '{0}' - expected '2'".format(rs.Size()))
print("::First() returned '{0}' - expected 'Test #1'".format(rs.First()))
print("::Next() returned '{0}' - expected 'Test #1'".format(rs.Next()))
print("::Last() returned '{0}' - expected 'Test #2'".format(rs.Last()))
