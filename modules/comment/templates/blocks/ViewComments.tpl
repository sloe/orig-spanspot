{*
 * $Revision: 16235 $
 * Read this before changing templates!  http://codex.gallery2.org/Gallery2:Editing_Templates
 *}

{* Set defaults *}
{if empty($item)} {assign var=item value=$theme.item} {/if}
{if empty($show)} {assign var=show value=3} {/if}
{g->callback type="comment.LoadComments" itemId=$item.id show=$show}
<meta property="fb:app_id" content="126757490732808">
<table><tr><td style="vertical-align:top">

<div id="fb-root"></div><script src="http://connect.facebook.net/en_US/all.js#xfbml=1"></script><fb:comments href="{g->url arg1="view=core.ShowItem" arg2="itemId=`$item.id`" forceFullUrl=true}" num_posts="20" width="400"></fb:comments>

</td><td style="vertical-align:top">
{if !empty($block.comment.LoadComments.comments)}
<div class="{$class}">
  <h3> {g->text text="Recent comments"} </h3>

  {if sizeof($block.comment.LoadComments.comments) < $block.comment.LoadComments.totalComments }
  <a href="{g->url arg1="view=comment.ShowComments" arg2="itemId=`$block.comment.LoadComments.item.id`"}">
    {g->text text="(view all %d comments)" arg1=$block.comment.LoadComments.totalComments}
  </a>
  {/if}

  {foreach from=$block.comment.LoadComments.comments item=comment}
    <div class="one-comment gcBorder2">
      {include file="gallery:modules/comment/templates/Comment.tpl"
	       user=$block.comment.LoadComments.commenters[$comment.commenterId]
	       comment=$comment
	       can=$block.comment.LoadComments.can
	       item=$block.comment.LoadComments.item
	       truncate=256}
    </div>
  {/foreach}
</div>
{/if}

</td></tr></table>
