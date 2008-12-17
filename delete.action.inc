<?php
// $Id$

function delete_action_info() {
  return array('delete_action' => array(
    'type' => 'node',
    'description' => t('Delete selected nodes'),
  ));
}

function delete_action($node) {
  node_delete($node->nid);
}

