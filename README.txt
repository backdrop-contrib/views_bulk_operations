QUICK START GUIDE
-----------------
1. Create a View.
2. Add a "Bulk operations" field if available (see technical details below).
3. Configure the field. There's a "Views Bulk Operations" fieldset where the
actions visible to the user are selected.
4. Go to the View page. VBO functionality should be present.

Note that your old (D6, pre-alpha D7) views that used the VBO style plugin
instead of the VBO field will need to be recreated.

TECHNICAL DETAILS
-----------------
The module relies on the Views 3 "form" feature available in all Views releases after May 28th 2011.
The selection field (checkbox / radio) is provided as a views field called "Bulk operations".
The field can use a relationship, allowing you to have a node view with
operations on node authors, for example.
Note that currently a View can only have one VBO field.
Even if the view has several VBO fields, only the first one found will be used.

The "Bulk operations" views field is available in Views for all entity base tables,
since VBO relies on entities directly and no longer has its own "object" abstraction.

There is Drush integration available, allowing you to run an action against the
resultset of any View that has the VBO field in its master display.

Operations are gathered from two sources:
1) Drupal Core Action API
2) Rules 2 (preferred).
The module also allows to use Batch API or the Drupal Queue to process the
selected entities, in order to avoid timeouts.

RULES 2 INTEGRATION
-------------------
The module can execute any created Rules component (rule, ruleset, action set)
that accepts an entity (example types: "node", "entity") or a list entity type
(example types: "list<node>", "list<entity>") as the first parameter.
As a reminder, Rules components can be created at admin/config/workflow/rules/components.

AGGREGATION
-----------
By default, VBO passes only one entity at a time to the operation.
This allows the entity loading to be segmented into chunks, avoiding memory
limits and timeouts. However, some operations need all selected entities to be
passed at once, which requires aggregation to be turned on.
For core actions that means setting "aggregate" => TRUE in your hook_action_info()
implementation. For Rules components, that means requiring a list entity type
such as list<node> as the first parameter.

Loading can't be segmented when aggregation is on, so the usual methods of
executing (Batch API, Drupal Queue) are bypassed and all entities are loaded
at once, making it possible to hit the memory limit.
That's why aggregation should only be enabled for actions that require a smaller
amount of items to be selected.
See the VBO action "Pass ids as arguments to a page" in
actions/argument_selector.action.inc for an example implementation.

EXECUTION METHODS
-----------------
When configuring the VBO field, the following setting can be seen:
"Number of entities to load at once", set to 10 by default.
When the number of selected items is less than that, the entities are laoded
all at once, and appropriate operations are fired.
When the number of selected items is more than that, Batch API is used to load
the entities in smaller groups (the size of which is taken from that setting),
and a progress bar is shown.

Alternatively, the user can choose to use the Drupal Queue, by enabling the
"Enqueue the operation instead of executing it directly" checkbox for each
desired action in the VBO field settings.
The entities and their operation are then enqueued one by one, to be processed
by a queue worker (which is usually cron).
This is useful for postponing long running operations.

EXAMPLE VIEWS
-------------
VBO comes with two default views, reimplementing the Content and User listings.
They are disabled by default. After enabling them at admin/structure/views
they can be accessed at admin/content2 and admin/people2.
