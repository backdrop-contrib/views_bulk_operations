<?php

function views_bulk_operations_delete_action_info() {
  return array(
    'views_bulk_operations_delete_node_action' => array(
      'type' => 'node',
      'label' => t('Delete node'),
      'configurable' => FALSE,
      'access operation' => VBO_ACCESS_OP_DELETE,
    ),
    'views_bulk_operations_delete_user_action' => array(
      'type' => 'user',
      'label' => t('Delete user'),
      'configurable' => FALSE,
      'access operation' => VBO_ACCESS_OP_DELETE,
    ),
    'views_bulk_operations_delete_comment_action' => array(
      'type' => 'comment',
      'label' => t('Delete comment'),
      'configurable' => FALSE,
      'access operation' => VBO_ACCESS_OP_DELETE,
    ),
    'views_bulk_operations_delete_term_action' => array(
      'type' => 'taxonomy_term',
      'label' => t('Delete term'),
      'configurable' => FALSE,
      'access operation' => VBO_ACCESS_OP_DELETE,
    ),
  );
}

function views_bulk_operations_delete_node_action($node) {
  node_delete($node->nid);
}

function views_bulk_operations_delete_user_action($user) {
  user_delete($user->uid);
}

function views_bulk_operations_delete_comment_action($comment) {
  comment_delete($comment->cid);
}

function views_bulk_operations_delete_term_action($term) {
  taxonomy_term_delete($term->tid);
}
