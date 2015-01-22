
Node Access Product

CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Configuration


INTRODUCTION
------------
Define nodes which have 'purchasable access'; these nodes cannot be seen by
anyone except for the Author, Administrators, and anyone who purchases the
relevant 'Node Access' product.  'Node Access' products grant view access to
'purchasable access' nodes, either by view, category, or node.  If the 'Node
Access' product is also a 'Recurring' product, then access will be granted 
for nodes published during the period of the recurring purchase.

Maintainer: Daniel Braksator (http://drupal.org/user/134005)

Module page: http://drupal.org/project/nap.


INSTALLATION
------------
1. Copy nap folder to modules directory.
2. At admin/modules enable the Node Access Product module and whichever
   submodule is supported by your store.
3. Optionally enable the taxonomy module to select content by category.
4. Optionally install the views module to select content by view.


CONFIGURATION
-------------
1. Enable permissions at admin/people/permissions.
2. Configure the module at the configuration section of your store.
3. Create or edit products and set the Node Access Product options.